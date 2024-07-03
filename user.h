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


class User
{
private:
    QString account="ddd";
    QString token="222";
    QString hashedPassword="123";
public:
    User();
    User(QString account, QString password) {
        this->account = account;
        this->hashedPassword = password;
    }
    QString login() {
        //https post a requests
        QNetworkAccessManager manager;
        QUrl url("http://www.mocklib.com/public/auth/login");
        QNetworkRequest request(url);
        QString data = QString("username=%1&password=%2").arg(this->account).arg(this->hashedPassword);
        QNetworkReply *reply = manager.get(request);
        //QNetworkReply *reply = manager.post(request, data.toUtf8());
        onFinished(reply);
        return token;
    }

    void onFinished(QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray responseData = reply->readAll();
            QString responseString = QString(responseData);
            qDebug() << responseString;
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
                }
            } else {
                qDebug() << "Invalid JSON format";
            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
    }
    //以上是未完成测试代码

    QString getUserHash() {
        QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
        return hash.toHex(); // 将散列值转换为十六进制字符串并返回
    }
};

#endif // USER_H
