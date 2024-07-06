#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardPaths>

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);

    bool initializeDatabase(const QString &name);
    bool userExists(const QString &account);
    bool insertUser(const QString &account, const QString &hashedPassword);
};

#endif // DATABASEMANAGER_H
