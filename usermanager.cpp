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
bool UserManager::updateUserInfo(User *user)
{
    // 保存更改到文件
    return saveToFile(user->getEmail(),user->encryptPassword(),true);
}

QString UserManager::getUserAccount()
{
    QFile file(filePath_);
    // 检查文件是否存在以及是否可以打开
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return ""; // 文件不存在或无法打开不是错误条件
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    // 检查JSON解析是否成功
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON file:" << error.errorString();
        return "";
    }

    QJsonObject json = doc.object();
    return json.value("act").toString();
}

QString UserManager::getUserPassWord(const QString &account)
{
    QFile file(filePath_);
    // 检查文件是否存在以及是否可以打开
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return ""; // 文件不存在或无法打开不是错误条件
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    // 检查JSON解析是否成功
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON file:" << error.errorString();
        return "";
    }

    QJsonObject json = doc.object();
    if( json.value("act") == account )
    {
        return decodePassword(account,json.value("psw").toString());
    }
    else
        return "";

}

// 将用户数据保存到文件
bool UserManager::saveToFile(const QString&account,const QString&password,const bool autoLogin)
{
    QJsonObject json;

    json["act"] = account;
    json["psw"] = password;
    json["auto"] = (autoLogin?"true":"false");

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

bool UserManager::getAutoLoginStaus()
{
    QFile file(filePath_);
    // 检查文件是否存在以及是否可以打开
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return ""; // 文件不存在或无法打开不是错误条件
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    // 检查JSON解析是否成功
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON file:" << error.errorString();
        return "";
    }

    QJsonObject json = doc.object();
    return json.value("auto").toString()=="true"?true:false;
}

bool UserManager::setAutoLoginStaus(bool autoLogin)
{
    // 打开文件
    QFile file(filePath_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << filePath_;
        return false;
    }

    // 读取文件内容到QByteArray
    QByteArray jsonData = file.readAll();
    file.close();

    // 将QByteArray转换为QJsonDocument
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 获取JSON对象
    QJsonObject jsonObj = jsonDoc.object();

    // 修改JSON对象的内容
    jsonObj["auto"] = (autoLogin?"true":"false");

    // 将修改后的JSON对象放回QJsonDocument
    jsonDoc.setObject(jsonObj);

    // 将修改后的JSON文档写回文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件以写入：" << filePath_;
        return false;
    }
    file.write(jsonDoc.toJson());
    file.close();
    return true;
}

QString UserManager::decodePassword(const QString &uuid,const QString &password){
    //以uuid为密钥rc4解密password
    QString result;
    for (int i = 0; i < password.size(); ++i) {
        ushort xor_result = static_cast<ushort>(password[i].unicode()) ^ static_cast<ushort>(uuid[i % uuid.size()].unicode());
        result.append(QChar(xor_result));
    }
    return result;
}
