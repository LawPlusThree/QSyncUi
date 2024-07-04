#include "mainwindow.h"
#include <QApplication>




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //测试用户
    User test("newuser@example.com","123456");
    test.login();

    MainWindow w;
    w.show();
    return a.exec();
}
