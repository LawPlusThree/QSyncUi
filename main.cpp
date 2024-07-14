#include <QApplication>
#include "ElaApplication.h"
#include "mainwindow.h"
#include <QLocalSocket>
#include <QLocalServer>
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ElaApplication::getInstance()->init();
	//read argv
	if (argc > 2) {
		qDebug() << "Received data from another instance:" << QString::fromLocal8Bit(argv[1]);
		qDebug() << "Received data from another instance:" << QString::fromLocal8Bit(argv[2]);
	}
	QLocalSocket socket;
	socket.connectToServer("QSyncUi");
	if (socket.waitForConnected(100)) {
		// 已有实例运行，发送数据给这个实例
		if (argc > 1) {
			QByteArray data(argv[1]);
			socket.write(data);
			socket.waitForBytesWritten();
		}
		return 0; // 退出程序
	}

	// 没有实例运行，创建QLocalServer来监听
	QLocalServer server;
	server.listen("QSyncUi");
	QObject::connect(&server, &QLocalServer::newConnection, [&server]() {
		QLocalSocket *newSocket = server.nextPendingConnection();
		QByteArray data;
		while (newSocket->waitForReadyRead(100)) {
			data.append(newSocket->readAll());
		}
		qDebug() << "Received data from another instance:" << QString::fromUtf8(data);
		// 根据接收到的数据处理逻辑，比如显示特定界面
	});

	MainWindow w;
	w.show();
	return a.exec();
}
