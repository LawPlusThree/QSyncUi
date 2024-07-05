#include "user.h"

User::User(QObject *parent)
    : QObject(parent)
    , manager(new QNetworkAccessManager(this))
{}

User::User(const QString &username, const QString &account, const QString &password, QObject *parent)
    : QObject(parent)
    , username(username)
    , account(account)
    , hashedPassword(password)
    , manager(new QNetworkAccessManager(this))
{}

void User::enroll()
{
    QUrl url("https://syncapi.snakekiss.com/register");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString postData
        = QString("username=%1&email=%2&password=%3").arg(username).arg(account).arg(hashedPassword);
    reply = manager->post(request, postData.toUtf8());

    connect(reply, &QNetworkReply::finished, this, &User::enrollFinished);
}

void User::login()
{
    QUrl url("https://syncapi.snakekiss.com/login");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString postData = QString("email=%1&password=%2").arg(account).arg(hashedPassword);
    reply = manager->post(request, postData.toUtf8());

    connect(reply, &QNetworkReply::finished, this, &User::loginFinished);
}

void User::forgetPassword()
{
    //找回密码功能
}

QString User::getUserHash() const
{
    QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
    return hash.toHex();
}

QString User::getSession()
{
    return session;
}

void User::enrollFinished()
{
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
    if (processCookies()) {
        emit enrollCompleted(session);
        qDebug() << "enrollfinished:" << session;
    }

    reply->deleteLater();
    disconnect(reply, &QNetworkReply::finished, this, &User::enrollFinished);
}

void User::loginFinished()
{
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
    if (processCookies()) {
        emit loginCompleted(session);
        qDebug() << "loginfinished:" << session;
    }

    reply->deleteLater();
    disconnect(reply, &QNetworkReply::finished, this, &User::loginFinished);
}

void User::processJsonObject(const QJsonObject &jsonObject)
{
    if (jsonObject.contains("code")) {
        qDebug() << "Value for code:" << jsonObject.value("code").toInt();
    }
    if (jsonObject.contains("message")) {
        qDebug() << "Value for message:" << jsonObject.value("message").toString();
    }
    if (jsonObject.contains("data")) {
        qDebug() << "Value for data:" << jsonObject.value("data").toString();
    }
}

bool User::processCookies()
{
    if (manager) {
        QNetworkCookieJar *cookieJar = manager->cookieJar();
        QList<QNetworkCookie> cookies = cookieJar->cookiesForUrl(reply->url());
        if (!cookies.isEmpty()) {
            qDebug() << "Received cookies:";
            for (const QNetworkCookie &cookie : cookies) {
                qDebug() << "Cookie Name:" << cookie.name();
                qDebug() << "Cookie Value:" << cookie.value();
                session = cookie.value();
                return true;
            }
        } else {
            qDebug() << "No cookies received.";
        }
    } else {
        qDebug() << "Failed to get QNetworkAccessManager.";
    }
    return false;
}
