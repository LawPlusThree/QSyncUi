#include <QApplication>
#include "ElaApplication.h"
#include "mainwindow.h"
#include <QLocalSocket>
#include <QLocalServer>
#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
	ElaApplication::getInstance()->init();
	//read argv
	QLocalSocket socket;

    QStringList arguments = a.arguments();
    int realArgc = arguments.size();

	socket.connectToServer("QSyncUi");
	if (socket.waitForConnected(100)) {
        if (realArgc > 1) {
            QByteArray data;
            for (int i = 1; i < realArgc; i++) {
                data.append(arguments.at(i).toUtf8());
                data.append('\n');
            }
			socket.write(data);
			socket.waitForBytesWritten();
		}
		return 0; // 退出程序
	}

    QVector<QString> argvs;
    QString sb("哦你");
    QString action;
    if(realArgc>1){
        action=a.arguments().at(1);
        for(int i=2;i<realArgc;i++){
            argvs.push_back(a.arguments().at(i));
        }
        MainWindow w(action,argvs);

        QLocalServer server;
        server.listen("QSyncUi");
        QObject::connect(&server, &QLocalServer::newConnection, [&server,&w]() {
            QLocalSocket *newSocket = server.nextPendingConnection();
            QByteArray data;
            while (newSocket->waitForReadyRead(100)) {
                data.append(newSocket->readAll());
            }
            QStringList list=QString::fromUtf8(data).split('\n');
            QString action=list.at(0);
            QVector<QString> argvs;
            for(int i=1;i<list.size();i++){
                argvs.push_back(list.at(i));
            }
            w.ArgvProcess(action,argvs);
        });

        w.show();
        return a.exec();
    }
	MainWindow w;
	w.show();
	return a.exec();
}
