#ifndef USER_H
#define USER_H
#include <QCryptographicHash>
#include <QDebug>
#include <QObject>
#include <QString>
#include "apirequest.h"
class User : public QObject
{
    Q_OBJECT

private:
    QString username; //username
    QString account;  //email
    QString data;
    QString hashedPassword; //password
    QString session;
    ApiRequest *apiRequest;
public:
    explicit User(QObject *parent = nullptr);
    User(const QString &username,
         const QString &account,
         const QString &password,
         QObject *parent = nullptr);
    User(const QString &account,
         const QString &password,
         QObject *parent = nullptr);
    void enroll();         //执行post请求，实现注册功能
    void login();          //执行post请求，实现登录功能
    void forgetPassword(); //执行post请求，实现找回密码功能

    QString getUserHash() const; //返回用户账户的哈希

    QString getSession(); //返回用户session

signals:
    void enrollCompleted(const QString &session); //注册成功的信号，发送获取到的cookie中session
    void loginCompleted(const QString &session); //登陆成功的信号，发送获取到的cookie中session

};

#endif // USER_H
