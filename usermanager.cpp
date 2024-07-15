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
    return saveToFile(user->getEmail(),user->encryptPassword(),true,getThread());
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
bool UserManager::saveToFile(const QString&account,const QString&password,const bool autoLogin,const int threadnum)
{
    //Read File, find diffs and then write to file
    QFile file(filePath_);
    // 检查文件是否存在以及是否可以打开
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return false; // 文件不存在或无法打开不是错误条件
    }
    QByteArray data = file.readAll();
    //读取为json对象
    QJsonObject json;
    json = QJsonDocument::fromJson(data).object();
    file.close();
    //只更新不同的数据
    if( json.value("act") != account )
    {
        json["act"] = account;
    }
    if( json.value("psw") != password )
    {
        json["psw"] = password;
    }
    if( json.value("auto") != (autoLogin?"true":"false") )
    {
        json["auto"] = (autoLogin?"true":"false");
    }
    if( json.value("trd") != threadnum )
    {
        json["trd"] = threadnum;
    }

    QJsonDocument doc(json);
    QFile file2(filePath_);
    // 打开文件用于写入
    if (!file2.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open file for writing:" << filePath_;
        return false;
    }

    // 将JSON文档转换为字符串并写入文件
    file2.write(doc.toJson());
    file2.close();
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

int UserManager::getThread()
{
    QFile file(filePath_); // 创建一个QFile对象，用于操作文件
    // 检查文件是否存在以及是否可以打开
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return 0; // 如果文件不存在或无法打开，则返回空字符串
    }

    QByteArray data = file.readAll(); // 读取文件的全部内容到一个QByteArray对象中
    file.close(); // 关闭文件

    QJsonParseError error; // 创建一个QJsonParseError对象，用于接收JSON解析的错误信息
    QJsonDocument doc = QJsonDocument::fromJson(data, &error); // 将读取的数据解析为一个QJsonDocument对象
    // 检查JSON解析是否成功
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse JSON file:" << error.errorString(); // 如果解析失败，打印错误信息
        return 0; // 并返回空字符串
    }

    QJsonObject json = doc.object(); // 从QJsonDocument对象中获取一个QJsonObject对象
    return json.value("trd").toInt(); // 从JSON对象中获取键为"trd"的值，并将其转换为整数返回
}

bool UserManager::setThread(int threadnum)
{
    // 打开文件
    QFile file(filePath_);
    // 尝试以只读模式打开文件，如果失败则打印错误信息并返回false
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << filePath_;
        return false;
    }

    // 读取文件内容到QByteArray对象中
    QByteArray jsonData = file.readAll();
    // 关闭文件
    file.close();

    // 将QByteArray对象转换为QJsonDocument对象
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);

    // 从QJsonDocument对象中获取JSON对象
    QJsonObject jsonObj = jsonDoc.object();

    // 修改JSON对象中对应的值
    jsonObj["trd"] = threadnum;

    // 将修改后的JSON对象重新设置到QJsonDocument对象中
    jsonDoc.setObject(jsonObj);

    // 尝试以只写模式重新打开文件，如果失败则打印错误信息并返回false
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件以写入：" << filePath_;
        return false;
    }
    // 将修改后的JSON文档写入文件
    file.write(jsonDoc.toJson());
    // 关闭文件
    file.close();
    // 返回true表示操作成功
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
