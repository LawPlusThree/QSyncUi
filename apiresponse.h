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
                QJsonObject datax):code(codex),message(messagex),data(datax){};
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
    QJsonValueRef getDatav(){
        return datav;
    }
    bool isSuccess();

private:
    void processJsonObject(const QJsonObject &jsonObject); //处理json内容
    int code;
    QString message;
    QJsonObject data;
    QJsonValueRef datav;
};

#endif // APIRESPONSE_H
