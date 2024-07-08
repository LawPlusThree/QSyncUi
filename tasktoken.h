#ifndef TASKTOKEN_H
#define TASKTOKEN_H
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>

class TaskToken
{
public:
    TaskToken(const QJsonObject &data);
    QString sessionToken;
    QString tmpSecretId;
    QString tmpSecretKey;
    QDateTime expiredTime;//过期时间的unix
};

#endif // TASKTOKEN_H
