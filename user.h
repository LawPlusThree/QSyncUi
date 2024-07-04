#ifndef USER_H
#define USER_H
#include <QString>
#include <QCryptographicHash>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDebug>
#include <QNetworkCookie>
#include <QNetworkCookieJar>


class User : public QObject {
    Q_OBJECT

private:
    QString account;
    QString data;
    QString hashedPassword;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString session;
public:
    User(QObject *parent = nullptr) : QObject(parent) {
        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &User::loginonFinished);
    }
    User(QString account, QString password, QObject *parent = nullptr) : QObject(parent) {
        this->account = account;
        this->hashedPassword = password;
        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &User::loginonFinished);
    }
    void login() {
        //https post a requests
        QUrl url("https://syncapi.snakekiss.com/login");
        QNetworkRequest request(url);
        // 设置请求头
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        request.setHeader(QNetworkRequest::UserAgentHeader, "MyApp/1.0");
        //请求data
        QString data = QString("email=%1&password=%2").arg(this->account).arg(this->hashedPassword);
        reply = manager->post(request, data.toUtf8());
    }
    QString getUserHash() {
        QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
        return hash.toHex(); // 将散列值转换为十六进制字符串并返回
    }
signals:
    void loginCompleted(const QString &session);//登陆完成的信号
private slots:
    void loginonFinished() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

            if (jsonDoc.isObject()) {
                QJsonObject jsonObject = jsonDoc.object();
                if (jsonObject.contains("code")) {
                    qDebug() << "Value for code: " << jsonObject.value("code").toString();
                }
                if (jsonObject.contains("message")) {
                    qDebug() << "Value for message: " << jsonObject.value("message").toString();
                }
                if (jsonObject.contains("data")) {
                    qDebug() << "Value for data: " << jsonObject.value("data").toString();
                }
            } else {
                qDebug() << "Invalid JSON format";
            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        // 通过 QNetworkAccessManager 获取 QNetworkCookieJar 并获取 cookies
        if (manager) {
            QNetworkCookieJar *cookieJar = manager->cookieJar();
            QList<QNetworkCookie> cookies = cookieJar->cookiesForUrl(reply->url());
            if (!cookies.isEmpty()) {
                qDebug() << "Received cookies:";
                for (const QNetworkCookie &cookie : cookies) {
                    qDebug() << "Cookie Name:" << cookie.name();
                    qDebug() << "Cookie Value:" << cookie.value();
                    this->session=cookie.value();
                    emit loginCompleted(session); // 发射信号,ui方建立connect进行接收
                }
            } else {
                qDebug() << "No cookies received.";
            }
        } else {
            qDebug() << "Failed to get QNetworkAccessManager.";
        }

        reply->deleteLater();
    }
};

#endif // USER_H
