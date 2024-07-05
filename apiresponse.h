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
    ApiResponse(int codex,QString messagex,
        QJsonObject datax,QJsonValue datavx):code(codex),message(messagex),data(datax),datav(datavx){};
    ApiResponse(const QByteArray &responseData);

    int getCode(){
        return code;
    }
    QString getMessage(){
        return message;
    }
    QJsonObject getData(){
        return data;
    }
    QJsonValue getDatav(){
        return datav;
    }
    bool isSuccess();

private:
    void processJsonObject(const QJsonObject &jsonObject); //处理json内容
    int code;
    QString message;
    QJsonObject data;
    QJsonValue datav;
};

#endif // APIRESPONSE_H
