#include "apirequest.h"

ApiResponse ApiRequest::get(const QString &path) {
    QNetworkRequest request(QUrl(baseUrl + path));
    QNetworkReply* reply = manager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() == QNetworkReply::NoError||reply->error()>=401) {
        QByteArray responseData = reply->readAll();
        ApiResponse response(responseData);
        return response;
    } else {
        qDebug() << "Error:" << reply->errorString();
        QString jsonString = R"({"code": 666, "message": "ApiRequest error!", "data": "Nooo!"})";
        QByteArray jsonData = jsonString.toUtf8();
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = document.object();
        ApiResponse errorResponse(666,"ApiRequest error!",jsonObject);
        return errorResponse;
    }
}

ApiResponse ApiRequest::post(const QString &path, const QByteArray &data) {
    QNetworkRequest request(QUrl(baseUrl + path));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply* reply = manager->post(request, data);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    if (reply->error() == QNetworkReply::NoError||reply->error()>=401) {
        QByteArray responseData = reply->readAll();
        ApiResponse response(responseData);
        return response;
    } else {
        qDebug() << "Error:" << reply->errorString();
        QString jsonString = R"({"code": 666, "message": "ApiRequest error!", "data": "Nooo!"})";
        QByteArray jsonData = jsonString.toUtf8();
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject jsonObject = document.object();
        ApiResponse errorResponse(666,"ApiRequest error!",jsonObject);
        return errorResponse;
    }
}
