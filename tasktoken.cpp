#include "tasktoken.h"

TaskToken::TaskToken(const QJsonObject &data)
{
    QJsonObject credentials=data.value("credentials").toObject();
    sessionToken = credentials.value("sessionToken").toString();
    tmpSecretId = credentials.value("tmpSecretId").toString();
    tmpSecretKey = credentials.value("tmpSecretKey").toString();
    qint64 qtUnixTimestamp = static_cast<qint64>(data.value("expiredTime").toInt());
    expiredTime = QDateTime::fromSecsSinceEpoch(qtUnixTimestamp);
}
