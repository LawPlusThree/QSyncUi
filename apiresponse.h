#ifndef APIRESPONSE_H
#define APIRESPONSE_H
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>

class ApiResponse
{
public:
    ApiResponse(QObject *parent = nullptr){};

    ApiResponse* processJson(const QByteArray &responseData); //处理json
    void processJsonObject(const QJsonObject &jsonObject); //处理json内容
    int getCode(){
        return code;
    }
    QString getMessage(){
        return message;
    }
    QJsonObject getData(){
        return data;
    }

private:
    int code;
    QString message;
    QJsonObject data;
};

#endif // APIRESPONSE_H
