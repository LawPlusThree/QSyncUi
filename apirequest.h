#ifndef APIREQUEST_H
#define APIREQUEST_H
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QEventLoop>
#include "apiresponse.h"
class ApiRequest : public QObject
{
    Q_OBJECT
public:
    explicit ApiRequest(QObject *parent = nullptr){
        manager = new QNetworkAccessManager(this);
    };

    // 设置 API 主域名
    void setBaseUrl(const QString& url) {
        baseUrl = url;
    }

    // 发起 GET 请求
    ApiResponse get(const QString& path);

    // 发起 POST 请求
    ApiResponse post(const QString& path, const QByteArray& data);

private:
    QString baseUrl;
    QNetworkAccessManager* manager;
};

#endif // APIREQUEST_H
