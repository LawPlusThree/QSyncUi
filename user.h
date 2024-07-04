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

    void enroll();//执行post请求，实现注册功能
    void login();//执行post请求，实现登录功能
    void forgetPassword();//执行post请求，实现找回密码功能

    QString getUserHash() const;//返回用户账户的哈希

    QString getSession();//返回用户session

signals:
    void loginCompleted(const QString& session);//登陆成功的信号，发送获取到的cookie中session

private slots:
    void loginFinished();//登录请求返回数据处理

private:
    void processJsonObject(const QJsonObject& jsonObject);//处理json

    void processCookies();//处理cookie
};

#endif // USER_H
