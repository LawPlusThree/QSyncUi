#include "loginwindow.h"
#include <QApplication>
#include "ElaApplication.h"

#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //测试用户
    User loginuser("newuser@example.com","123456");
    loginuser.login();




    ElaApplication::getInstance()->init();
    MainWindow w;
    w.show();


    return a.exec();
}
