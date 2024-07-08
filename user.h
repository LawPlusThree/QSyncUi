#ifndef USER_H
#define USER_H
#include <QCryptographicHash>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonArray>
#include "apirequest.h"
#include "synctask.h"
class User : public QObject
{
    Q_OBJECT

private:
    QString username; //username
    QString account;  //email
    QString data;
    QString hashedPassword; //password
    bool isLogin=false;
    ApiRequest *apiRequest;
    QVector<SyncTask> tasks;
public:
    User(const QString &username,
         const QString &account,
         const QString &password,
         QObject *parent = nullptr);
    User(const QString &account,
         const QString &password,
         QObject *parent = nullptr);
    User(const User &user);
    bool enroll();         //执行post请求，实现注册功能
    bool login();          //执行post请求，实现登录功能
    bool forgetPassword(); //执行post请求，实现找回密码功能
    bool loadTask();//获取云端task
    bool updateUser(User &user);
    QString getS3Location();//获取云端容器地址
    QString getUsername();//获取用户名
    QString getEmail();//获取用户邮箱
    QString getUserHash() const; //返回用户账户的哈希
    QString gethashedPassword();
    bool getisLogin(); //返回用户登陆状态

signals:
    void enrollCompleted(const QString &session); //注册成功的信号，发送获取到的cookie中session
    void loginCompleted(const QString &session); //登陆成功的信号，发送获取到的cookie中session

};

#endif // USER_H
