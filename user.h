#ifndef USER_H
#define USER_H
#include <QCryptographicHash>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonArray>
#include <QImageReader>
#include <QImage>
#include <QBuffer>
#include <QByteArray>
#include "apirequest.h"
#include "synctask.h"
#include "tasktoken.h"
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
public:
    QString avatarpath;

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
    bool enroll(const QString &avatarpath);//注册带头像
    bool login();          //执行post请求，实现登录功能
    bool forgetPassword(); //执行post请求，实现找回密码功能
    bool updateAvatar(const QString &filePath);//更新头像
    bool addTask(const QString& localDir, const QString& s3Dir, int syncType, int usedSize, int totalSize);//添加云端task
    QVector<SyncTask> getTask();//获取云端task
    TaskToken getTaskToken(int id);//获取tasktoken
    TaskToken getTaskTokenByRemote(QString s3Dir);
    bool logout();//登出
    bool updateUser(User &user);
    QString getS3Location();//获取云端容器地址
    QString getUsername();//获取用户名
    QString getEmail();//获取用户邮箱
    QString getUserHash() const; //返回用户账户的哈希
    QString gethashedPassword();
    QString getAvatarPath();//获取头像路径
    bool getisLogin(); //返回用户登陆状态

signals:
    void enrollResponse(const int &code,const QJsonObject &data,const QString &message); //注册的信号
    void loginResponse(const int &code,const QJsonObject &data,const QString &message); //登陆的信号
    void addTaskResponse(const int &code,const QJsonObject &data,const QString &message); //新建任务的信号
    void updateAvatarResponse(const int &code,const QJsonObject &data,const QString &message);//改头像的信号
    void logoutResponse(const int &code,const QJsonObject &data,const QString &message);

};

#endif // USER_H
