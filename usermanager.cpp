#include "usermanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QDir>
#include <QJsonParseError>

// UserManager类的构造函数
UserManager::UserManager(const QString &filePath, QObject *parent) : QObject(parent), filePath_(filePath)
{
    // 确保数据文件夹存在
    QDir().mkpath(QFileInfo(filePath_).absolutePath());
    // 从文件中加载用户数据
    loadFromFile();
}

// 插入新用户
bool UserManager::insertUser(const QString &account, const QString &hashedPassword)
{
    // 检查账号是否已存在
    if (userMap_.contains(account)) {
        return false;
    }

    // 将新用户信息添加到映射中
    userMap_[account] = hashedPassword;
    // 将映射保存到文件
    return saveToFile();
}

// 更新用户信息
bool UserManager::updateUserInfo(const QString &account, const QString &newHashedPassword)
{
    // 检查账号是否存在
    if (!userMap_.contains(account)) {
        return false;
    }

    // 更新用户密码
    userMap_[account] = newHashedPassword;
    // 保存更改到文件
    return saveToFile();
}

// 获取所有账号
QList<QString> UserManager::getAllAccounts()
{
    // 返回映射中的所有账号
    return userMap_.keys();
}

// 获取用户账号和密码
QPair<QString, QString> UserManager::getUserPassword(const QString &account)
{
    QPair<QString, QString> userPassword(account, "");
    // 检查账号是否存在并获取密码
    if (userMap_.contains(account)) {
        userPassword.second = userMap_[account];
    }
    return userPassword;
}

// 将用户数据保存到文件
bool UserManager::saveToFile()
{
    QJsonObject json;
    // 将映射中的数据转换为JSON对象
    for (auto it = userMap_.constBegin(); it != userMap_.constEnd(); ++it) {
        json[it.key()] = it.value();
    }

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

// 从文件加载用户数据
bool UserManager::loadFromFile()
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
    // 将JSON对象中的数据加载到映射中
    for (auto it = json.begin(); it != json.end(); ++it) {
        userMap_[it.key()] = it.value().toString();
    }

    return true;
}
