#include "user.h"

User::User(const QString &username, const QString &account, const QString &password, QObject *parent)
    : QObject(parent)
    , username(username)
    , account(account)
    , hashedPassword(password)
{
    apiRequest=new ApiRequest(this);
    apiRequest->setBaseUrl("https://syncapi.snakekiss.com");
}

User::User(const QString &account, const QString &password, QObject *parent)
    : QObject(parent)
    , account(account)
    , hashedPassword(password)
{
    apiRequest=new ApiRequest(this);
    apiRequest->setBaseUrl("https://syncapi.snakekiss.com");
}

bool User::enroll()
{
    QString postData
    = QString("username=%1&email=%2&password=%3").arg(username).arg(account).arg(hashedPassword);
    ApiResponse response=apiRequest->post("/register",postData.toUtf8());
    return response.isSuccess();
}

bool User::login()
{
    QString postData = QString("email=%1&password=%2").arg(account).arg(hashedPassword);
    ApiResponse response=apiRequest->post("/login",postData.toUtf8());
    if(response.isSuccess()){
        isLogin=true;
    }
    else if(response.getCode()==403){
        isLogin=false;

    }
    return response.isSuccess();
}

bool User::forgetPassword()
{
    //找回密码功能
    return true;
}

bool User::loadTask()
{
    //获取云端task
    ApiResponse response=apiRequest->get("/tasks");
    QJsonArray tasksArray = response.getDatav().toArray();

    for (int i = 0; i < tasksArray.size(); ++i) {
        QJsonObject taskObj = tasksArray[i].toObject();
        SyncTask task(taskObj["localDir"].toString(),
                taskObj["s3Dir"].toString(),taskObj["syncType"].toInt());
        //task.usedSize = taskObj["usedSize"].toDouble();
        //task.totalSize = taskObj["totalSize"].toDouble();
        tasks.push_back(task);
    }
    return response.isSuccess();
}

QString User::getS3Location()
{
    if(isLogin){
        ApiResponse response=apiRequest->get("/s3/Info");
        return response.getData().value("endpoint").toString();
    }
    else{
        qDebug()<<"pls login before get s3 location!";
        return "";
    }
}

QString User::getUserHash() const
{
    QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
    return hash.toHex();
}

/*QString User::getSession()
{
   // return session;
}*/

