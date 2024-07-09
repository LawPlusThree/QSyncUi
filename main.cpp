#include <QApplication>
#include "ElaApplication.h"
#include "synctask.h"
#include "mainwindow.h"
#include "synccore.h"
#include "signhelper.h"
#include "tasktoken.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //测试用户
    User loginuser("enrolluser@example.com","123456");
    loginuser.login();
    TaskToken tt=loginuser.getTaskTokenByRemote("home/");
    SignHelper sh("100037680555",tt.tmpSecretId,tt.tmpSecretKey);
    //request: https://qsync-1320107701.cos.ap-nanjing.myqcloud.com/?prefix=home
    QNetworkRequest request(QUrl("https://qsync-1320107701.cos.ap-nanjing.myqcloud.com/?prefix=home"));
    request.setAttribute(QNetworkRequest::CustomVerbAttribute,"GET");
    sh.generateSignature(request,60);
    //qDebug() <<"s3 location:"<< loginuser.getS3Location();
    SyncTaskDatabaseManager stm(&loginuser);
    SyncTask mytask("E:/","/home/",1);
    SyncCore sc(&a);
    //sc.addTask(&mytask);
    //stm.addTask(mytask);
    //qDebug()<<stm.queryTask(mytask);
    ElaApplication::getInstance()->init();
    MainWindow w;
    w.show();


    return a.exec();
}
