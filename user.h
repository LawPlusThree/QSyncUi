#ifndef USER_H
#define USER_H
#include <QString>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QObject>
#include <QString>
#include <QDebug>

class User : public QObject {
    Q_OBJECT

private:
    QString account;
    QString data;
    QString hashedPassword;
    QNetworkAccessManager* manager;
    QNetworkReply* reply;
    QString session;

public:
    explicit User(QObject* parent = nullptr) : QObject(parent), manager(new QNetworkAccessManager(this)) {
        connect(manager, &QNetworkAccessManager::finished, this, &User::loginFinished);
    }

    User(const QString& account, const QString& password, QObject* parent = nullptr) : QObject(parent), account(account), hashedPassword(password), manager(new QNetworkAccessManager(this)) {
        connect(manager, &QNetworkAccessManager::finished, this, &User::loginFinished);
    }

    void login() {
        QUrl url("https://syncapi.snakekiss.com/login");
        QNetworkRequest request(url);

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setHeader(QNetworkRequest::UserAgentHeader, "MyApp/1.0");

        QString postData = QString("email=%1&password=%2").arg(account, hashedPassword);
        reply = manager->post(request, postData.toUtf8());
    }

    QString getUserHash() const {
        QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
        return hash.toHex();
    }

    QString getSession(){
        return session;
    }

signals:
    void loginCompleted(const QString& session);//登陆成功的信号，发送获取到的cookie

private slots:
    void loginFinished() {
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

private:
    void processJsonObject(const QJsonObject& jsonObject) {
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

    void processCookies() {
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
};

#endif // USER_H
