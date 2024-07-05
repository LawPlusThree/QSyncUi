#include "Apiresponse.h"

ApiResponse* ApiResponse::processJson(const QByteArray &responseData)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();
        processJsonObject(jsonObject);
    } else {
        qDebug() << "Invalid JSON format";
    }
    return this;
}

void ApiResponse::processJsonObject(const QJsonObject &jsonObject)
{
    if (jsonObject.contains("code")) {
        qDebug() << "Value for code:" << jsonObject.value("code").toInt();
        code=jsonObject.value("code").toInt();
    }
    if (jsonObject.contains("message")) {
        qDebug() << "Value for message:" << jsonObject.value("message").toString();
        message=jsonObject.value("message").toString();
    }
    if (jsonObject.contains("data")) {
        qDebug() << "Value for data:" << jsonObject.value("data").toObject();
        data=jsonObject.value("data").toObject();
    }
}
