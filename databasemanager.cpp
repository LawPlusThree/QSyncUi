#include "databasemanager.h"
#include "qsqlerror.h"

DatabaseManager::DatabaseManager(QObject *parent):QObject(parent)
{
}

bool DatabaseManager::initializeDatabase(const QString &name)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + name +".db");
    if(!db.open())
    {
        qWarning() << "Database open failed:" << db.lastError();
        return false;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS Users (id INTEGER PRIMARY KEY AUTOINCREMENT, account TEXT UNIQUE, password TEXT)")) {
        qWarning() << "Failed to create table:" << query.lastError();
        return false;
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

    return true;
}

bool DatabaseManager::userExists(const QString &account)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM Users WHERE account = :account");
    query.bindValue(":account", account);

    if (!query.exec()) {
        qWarning() << "Failed to check user existence:" << query.lastError();
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}
