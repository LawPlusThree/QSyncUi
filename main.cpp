#include <QApplication>
#include "ElaApplication.h"
#include "synctask.h"
#include "mainwindow.h"
#include "synccore.h"
#include "signhelper.h"
#include "tasktoken.h"
#include "cosclient.h"
#include "xmlprocesser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //测试用户
    User loginuser("enrolluser@example.com","123456");
    loginuser.login();
    TaskToken tt=loginuser.getTaskTokenByRemote("home/");
    //request: https://qsync-1320107701.cos.ap-nanjing.myqcloud.com/?prefix=home
    QNetworkRequest request(QUrl("https://qsync-1320107701.cos.ap-nanjing.myqcloud.com/?prefix=home"));
    request.setAttribute(QNetworkRequest::CustomVerbAttribute,"GET");
    QString bucketName="qsync";
    QString appId="1320107701";
    QString region="ap-nanjing";
    QString secretId=tt.tmpSecretId;
    QString secretKey=tt.tmpSecretKey;
    QString token=tt.sessionToken;
    QDateTime expiredTime=tt.expiredTime;
    COSClient cosclient(bucketName,appId,region,secretId,secretKey,token,expiredTime);
    QString xmlstr=cosclient.listObjects("home/","");
    XmlProcesser xp;
    Bucket bucket=xp.processXml(xmlstr);

    //sh.generateSignature(request,60);
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
