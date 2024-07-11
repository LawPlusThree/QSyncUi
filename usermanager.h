#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QPair>
#include"user.h"

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);
    bool updateUserInfo(User *user);//保存登陆成功后的用户信息
    QString getUserAccount();
    QString getUserPassWord(const QString &account);
    bool saveToFile(const QString&,const QString&,const bool);
    bool getAutoLoginStaus();
    bool setAutoLoginStaus(bool autoLogin);

private:
    QString filePath_ = "usif.json";
    QString decodePassword(const QString &uuid,const QString &password);
};
#endif // USERMANAGER_H
