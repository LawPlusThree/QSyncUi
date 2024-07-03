#include "mainwindow.h"

#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    User test("11","22");
    QString result = test.login();

    MainWindow w;
    w.show();
    return a.exec();
}
