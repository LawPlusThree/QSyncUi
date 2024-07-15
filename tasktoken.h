#ifndef TASKTOKEN_H
#define TASKTOKEN_H
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

struct TaskToken
{
public:
    TaskToken(){}
    TaskToken(const QJsonObject &data);
    TaskToken(const TaskToken &other){
        sessionToken = other.sessionToken;
        tmpSecretId = other.tmpSecretId;
        tmpSecretKey = other.tmpSecretKey;
        expiredTime = other.expiredTime;
    }
    bool isExpired(){
        qDebug()<<"expiredTime"<<expiredTime<<QDateTime::currentDateTime();
        return QDateTime::currentDateTime()>expiredTime;
    }
    QString sessionToken;
    QString tmpSecretId;
    QString tmpSecretKey;
    QDateTime expiredTime;//过期时间的unix
};

#endif // TASKTOKEN_H
