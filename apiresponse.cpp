#include "Apiresponse.h"


ApiResponse::ApiResponse(const QByteArray &responseData){
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    if (jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();
        processJsonObject(jsonObject);
    } else {
        qDebug() << "Invalid JSON format";
    }
}

bool ApiResponse::isSuccess()
{
    if(code==200||code==201)
        return true;
    else return false;
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
