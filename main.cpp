#include "loginwindow.h"
#include <QApplication>
#include "ElaApplication.h"
#include "synctask.h"
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //测试用户
    User loginuser("newuser@example.com","123456");
    loginuser.login();
    SyncTaskManager stm(loginuser);
    SyncTask mytask(1,"/home/","/home/",1);
    //stm.addTask(mytask);
    qDebug()<<stm.queryTask(mytask);
    ElaApplication::getInstance()->init();
    MainWindow w;
    w.show();


    return a.exec();
}
