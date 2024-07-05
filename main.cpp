#include "loginwindow.h"
#include <QApplication>
#include "ElaApplication.h"
#include "synctask.h"
#include "mainwindow.h"
#include "synccore.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //测试用户
    User loginuser("enrolluser@example.com","123456");
    loginuser.login();
    SyncTaskManager stm(&loginuser);
    SyncTask mytask("D:/","/home/",1);
    SyncCore sc(&a);
    sc.addTask(&mytask);
    //stm.addTask(mytask);
    qDebug()<<stm.queryTask(mytask);
    ElaApplication::getInstance()->init();
    MainWindow w;
    w.show();


    return a.exec();
}
