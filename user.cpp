#include "user.h"


User::User(QObject *parent) : QObject(parent), manager(new QNetworkAccessManager(this)) {
    connect(manager, &QNetworkAccessManager::finished, this, &User::loginFinished);
}

User::User(const QString &account, const QString &password, QObject *parent) : QObject(parent), account(account), hashedPassword(password), manager(new QNetworkAccessManager(this)) {
    connect(manager, &QNetworkAccessManager::finished, this, &User::loginFinished);
}

void User::login() {
    QUrl url("https://syncapi.snakekiss.com/login");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setHeader(QNetworkRequest::UserAgentHeader, "MyApp/1.0");

    QString postData = QString("email=%1&password=%2").arg(account, hashedPassword);
    reply = manager->post(request, postData.toUtf8());
}

QString User::getUserHash() const {
    QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
    return hash.toHex();
}

QString User::getSession(){
    return session;
}

void User::loginFinished() {
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (jsonDoc.isObject()) {
            QJsonObject jsonObject = jsonDoc.object();
            processJsonObject(jsonObject);
        } else {
            qDebug() << "Invalid JSON format";
        }
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    processCookies();
    reply->deleteLater();
}

void User::processJsonObject(const QJsonObject &jsonObject) {
    if (jsonObject.contains("code")) {
        qDebug() << "Value for code:" << jsonObject.value("code").toString();
    }
    if (jsonObject.contains("message")) {
        qDebug() << "Value for message:" << jsonObject.value("message").toString();
    }
    if (jsonObject.contains("data")) {
        qDebug() << "Value for data:" << jsonObject.value("data").toString();
    }
}

void User::processCookies() {
    if (manager) {
        QNetworkCookieJar* cookieJar = manager->cookieJar();
        QList<QNetworkCookie> cookies = cookieJar->cookiesForUrl(reply->url());
        if (!cookies.isEmpty()) {
            qDebug() << "Received cookies:";
            for (const QNetworkCookie& cookie : cookies) {
                qDebug() << "Cookie Name:" << cookie.name();
                qDebug() << "Cookie Value:" << cookie.value();
                session = cookie.value();
                emit loginCompleted(session);
            }
        } else {
            qDebug() << "No cookies received.";
        }
    } else {
        qDebug() << "Failed to get QNetworkAccessManager.";
    }
}
