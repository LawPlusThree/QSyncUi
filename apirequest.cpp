#include "apirequest.h"

QByteArray ApiRequest::get(const QString &path) {
    QNetworkRequest request(QUrl(baseUrl + path));
    QNetworkReply* reply = manager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        response=response->processJson(responseData);
    } else {
        qDebug() << "Error:" << reply->errorString();
    }
    return 0;
}

QByteArray ApiRequest::post(const QString &path, const QByteArray &data) {
    QNetworkRequest request(QUrl(baseUrl + path));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply* reply = manager->post(request, data);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        response=response->processJson(responseData);
        qDebug() << "response code:" << response->getCode();
    } else {
        qDebug() << "Error:" << reply->errorString();
    }
    return 0;
}
