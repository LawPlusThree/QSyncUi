#include "databasemanager.h"
#include "qsqlerror.h"
#include<QMap>

DatabaseManager::DatabaseManager(QObject *parent):QObject(parent)
{
    initializeDatabase();
}

bool DatabaseManager::initializeDatabase()
{
    /*
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //存到userData文件夹

    db.setDatabaseName(name+"syncTasks.db");
    if (!db.open()) {
        qDebug() << "Database open failed:" << db.lastError();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS SyncTasks (id INTEGER PRIMARY KEY AUTOINCREMENT, localPath TEXT, remotePath TEXT, syncStatus INTEGER)")) {
        qDebug() << "Failed to create table:" << query.lastError();
    }*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("thxDatabase.db");
    if(!db.open())
    {
        qWarning() << "Database open failed:" << db.lastError();
        return false;
    }

    QSqlQuery query;

    if (!query.exec("CREATE TABLE IF NOT EXISTS Users (account TEXT PRIMARY KEY, hashedPassword TEXT);")) {
        qWarning() << "Failed to create table:" << query.lastError();
        return false;
    }

    query.prepare("SELECT account, hashedPassword FROM Users");
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


    return true;
}

bool DatabaseManager::insertUser(const QString &account, const QString &hashedPassword)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Users (account, hashedPassword) VALUES (:account, :hashedPassword)");
    query.bindValue(":account", account);
    query.bindValue(":hashedPassword", hashedPassword);

    if (!query.exec()) {
        qWarning() << "Failed to insert user:" << query.lastError();
        return false;
    }

    // 成功插入新用户后，更新账号密码映射
    accountPasswordMap_[account] = hashedPassword;

    return true;
}

bool DatabaseManager::updateUserInfo(const QString &account,const QString &newHashedPassword)
{
    QSqlQuery query;
    bool transactionSuccess = false;
    // 确保数据库连接是打开的
    if (!QSqlDatabase::database().isOpen()) {
        qWarning() << "Database is not open.";
        return false;
    }

    // 开始一个事务
    if (QSqlDatabase::database().transaction()) {
        // 准备 SQL 语句
        query.prepare("UPDATE Users SET hashedPassword = :newHashedPassword WHERE account = :account");
        // 绑定参数
        query.bindValue(":account", account);
        query.bindValue(":newHashedPassword", newHashedPassword);
        // 执行查询
        if (!query.exec()) {
            // 如果执行失败，打印错误信息
            qWarning() << "Failed to update user info:" << query.lastError().text();
            // 回滚事务
            QSqlDatabase::database().rollback();
            return false;
        } else {
            // 数据库更新成功，更新映射
            accountPasswordMap_[account] = newHashedPassword;
            // 提交事务
            transactionSuccess = QSqlDatabase::database().commit();
            if (!transactionSuccess) {
                qWarning() << "Failed to commit transaction:" << QSqlDatabase::database().lastError().text();
            }
        }
    } else {
        qWarning() << "Failed to start transaction:" << QSqlDatabase::database().lastError().text();
        return false;
    }

    return transactionSuccess;
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
