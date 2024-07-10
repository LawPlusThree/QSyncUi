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
    loginuser.enroll();
    loginuser.login();


    //loginuser.addTask("E:/","home/",1,22,22);
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
    QMap<QString, QString> myMap;
    headHeader hh;
    //qDebug()<<"save to local:"<<cosclient.save2Local("home/1.txt","C:/Users/work123/Desktop/1.txt","",myMap);
    /*QMap<QString,QString> metaDatas;
    QMap<QString,QString> headers=cosclient.headObject("home/1.txt","C:/Users/work123/Desktop/1.txt","",hh);
    for(auto it=headers.begin();it!=headers.end();it++){
        if(it.key().startsWith("x-cos-meta-")){
            metaDatas[it.key().mid(11)]=it.value();
        }
    }
    qDebug()<<"metaDatas:"<<metaDatas;*/
    //qDebug()<<cosclient.deleteObject("/home/1.jpg","");


    //QString xmlstr=cosclient.listObjects("home/","");
    //qDebug()<<cosclient.putLocalObject("/home/1.jpg","C:/Users/33327/Documents/Tencent Files/3332770186/Image/Group2/LN/[B/LN[BMJILV2RA]X6$X49XPGS.jpg");
    //qDebug()<<cosclient.multiUpload("/home/2.mp4","E:\\Game Recording\\Yuan Shen 原神\\3.mp4");
    //XmlProcesser xp;
    //Bucket bucket=xp.processXml(xmlstr);
/*
    //sh.generateSignature(request,60);
    //qDebug() <<"s3 location:"<< loginuser.getS3Location();
    SyncTaskDatabaseManager stm(&loginuser);
    SyncTask mytask("E:/","/home/",1);
    SyncCore sc(&a);
    //sc.addTask(&mytask);
    //stm.addTask(mytask);
    //qDebug()<<stm.queryTask(mytask);*/

    ElaApplication::getInstance()->init();
    MainWindow w;
    w.show();


    return a.exec();
}
