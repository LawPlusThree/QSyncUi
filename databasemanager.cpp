#include "databasemanager.h"
#include "qsqlerror.h"
#include<QMap>

DatabaseManager::DatabaseManager(QObject *parent):QObject(parent)
{
    //初始化时填充账号密码映射
    QSqlQuery query;
    QMap<QString,QString>accountPasswordMap;

    // 将账号密码映射存储在类成员变量中
    accountPasswordMap_ = accountPasswordMap;
}

bool DatabaseManager::initializeDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.db");
    if(!db.open())
    {
        qWarning() << "Database open failed:" << db.lastError();
        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT account, hashedPassword, avatar FROM Users");
    if(query.exec())
    {
        while (query.next()) {
            accountPasswordMap_[query.value(0).toString()] = query.value(1).toString();
        }
    }
    else
    {
        qWarning() << "Failed to get all accounts:" << query.lastError();
    }

    if (!query.exec("CREATE TABLE IF NOT EXISTS Users (id INTEGER PRIMARY KEY AUTOINCREMENT, account TEXT UNIQUE, hashedPassword TEXT, avatar TEXT)")) {
        qWarning() << "Failed to create table:" << query.lastError();
        return false;
    }

    return true;
}

bool DatabaseManager::insertUser(const QString &account, const QString &hashedPassword, const QString &avatar)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Users (account, hashedPassword, avatar) VALUES (:account, :hashedPassword, :avatar)");
    query.bindValue(":account", account);
    query.bindValue(":hashedPassword", hashedPassword);
    query.bindValue(":avatar", avatar);

    if (!query.exec()) {
        qWarning() << "Failed to insert user:" << query.lastError();
        return false;
    }

    return true;
}

bool DatabaseManager::updateUserInfo(const QString &account,const QString &newHashedPassword, const QString &newAvatar)
{
    QSqlQuery query;
    query.prepare("UPDATE Users SET hashedPassword = :newHashedPassword, avatar = :newAvatar WHERE account = :account");
    query.bindValue(":newHashedPassword", newHashedPassword);
    query.bindValue(":newAvatar", newAvatar);
    query.bindValue(":account", account);

    if (!query.exec()) {
        qWarning() << "Failed to update user info:" << query.lastError();
        return false;
    }

    // 更新账号密码映射
    accountPasswordMap_[account] = newHashedPassword;

    return true;
}

QList<QString> DatabaseManager::getAllAccounts()
{
    QSqlQuery query;
    QList<QString> accounts;

    query.prepare("SELECT account FROM Users");
    if (query.exec()) {

        while (query.next()) {
            accounts.append(query.value(0).toString());
        }
    } else {
        qWarning() << "Failed to get all accounts:" << query.lastError();
    }

    return accounts;
}

QPair<QString, QString> DatabaseManager::getUserPassword(const QString &account)
{
    QPair<QString, QString> userPassword;
    userPassword.first = account; // 账号

    if (accountPasswordMap_.contains(account)) {
        userPassword.second = accountPasswordMap_[account]; // 密码
        return userPassword;
    }

    // 如果没有找到匹配的账号，返回空值
    return userPassword;
}

QString DatabaseManager::getUserAvatar(const QString &account)
{
    QSqlQuery query;
    query.prepare("SELECT avatar FROM Users WHERE account = :account");
    query.bindValue(":account", account);

    if (query.exec()) {
        if (query.next()) {
            return query.value(0).toString();
        }
    } else {
        qWarning() << "Failed to get user avatar:" << query.lastError();
    }

    return QString();
}
