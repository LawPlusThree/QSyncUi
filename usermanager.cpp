#include "usermanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDir>
#include <QJsonParseError>

// UserManager类的构造函数
UserManager::UserManager( QObject *parent) : QObject(parent){}

//保存登陆成功后的用户信息
bool UserManager::updateUserInfo(User &user)
{
    // 保存更改到文件
    return saveToFile(user.getEmail(),user.encryptPassword());
}

QString UserManager::getUserPassWord(const QString &account)
{
    QFile file(filePath_);
    // 检查文件是否存在以及是否可以打开
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return true; // 文件不存在或无法打开不是错误条件
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    // 检查JSON解析是否成功
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON file:" << error.errorString();
        return false;
    }

    QJsonObject json = doc.object();


    return true;
}

// 将用户数据保存到文件
bool UserManager::saveToFile(const QString&account,const QString&password)
{
    QJsonObject json;

    json["act"] = account;
    json["psw"] = password;

    QJsonDocument doc(json);
    QFile file(filePath_);
    // 打开文件用于写入
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath_;
        return false;
    }

    // 将JSON文档转换为字符串并写入文件
    file.write(doc.toJson());
    file.close();
    return true;
}
