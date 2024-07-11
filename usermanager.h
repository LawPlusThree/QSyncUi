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
    QString getUserPassWord(const QString &account);

private:
    QString filePath_ = "usif.json";
    bool saveToFile(const QString&,const QString&);
    QString decodePassword(const QString &password); 
};
#endif // USERMANAGER_H
