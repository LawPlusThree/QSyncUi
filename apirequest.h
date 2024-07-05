#ifndef APIREQUEST_H
#define APIREQUEST_H
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QEventLoop>
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
    QByteArray get(const QString& path) {
        QNetworkRequest request(QUrl(baseUrl + path));
        QNetworkReply* reply = manager->get(request);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        return reply->readAll();
    }

    // 发起 POST 请求
    QByteArray post(const QString& path, const QByteArray& data) {
        QNetworkRequest request(QUrl(baseUrl + path));
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        QNetworkReply* reply = manager->post(request, data);
        QEventLoop loop;
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
        return reply->readAll();
    }

private:
    QString baseUrl;
    QNetworkAccessManager* manager;
};

#endif // APIREQUEST_H
