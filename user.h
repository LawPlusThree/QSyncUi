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


class User : public QObject {
    Q_OBJECT

private:
    QString account;
    QString token;
    QString hashedPassword;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
public:
    User(QObject *parent = nullptr) : QObject(parent) {
        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &User::onFinished);
    }
    User(QString account, QString password, QObject *parent = nullptr) : QObject(parent) {
        this->account = account;
        this->hashedPassword = password;
        manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, &User::onFinished);
    }
    void login() {
        //https post a requests
        QUrl url("https://www.mocklib.com/mock/public/auth/login");
        QNetworkRequest request(url);
        QString data = QString("username=%1&password=%2").arg(this->account).arg(this->hashedPassword);
        //reply = manager->post(request, data.toUtf8());
        reply = manager->get(request);
    }
    QString getUserHash() {
        QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
        return hash.toHex(); // 将散列值转换为十六进制字符串并返回
    }
signals:
    void loginCompleted(const QString &token);//登陆完成的信号
private slots:
    void onFinished() {
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
                if (jsonObject.contains("token")) {
                    qDebug() << "Value for token: " << jsonObject.value("token").toString();
                    this->token = jsonObject.value("token").toString();
                    emit loginCompleted(token); // 发射信号,ui方建立connect进行接收
                }
            } else {
                qDebug() << "Invalid JSON format";
            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
    }
};

#endif // USER_H
