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
    explicit User(QObject* parent = nullptr);

    User(const QString& account, const QString& password, QObject* parent = nullptr);

    void login();

    QString getUserHash() const;

    QString getSession();

signals:
    void loginCompleted(const QString& session);//登陆成功的信号，发送获取到的cookie

private slots:
    void loginFinished();

private:
    void processJsonObject(const QJsonObject& jsonObject);

    void processCookies();
};

#endif // USER_H
