#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardPaths>
#include<QPair>
#include<QList>

class DatabaseManager : public QObject
{
    Q_OBJECT
private:
    QMap<QString,QString>accountPasswordMap_;
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    bool updateUserInfo(const QString &account, const QString &newHashedPassword, const QString &newAvatar);
    bool initializeDatabase();
    bool insertUser(const QString &account, const QString &hashedPassword, const QString &avatar = "E:\24SummerTraining\avatar_placeholder.png");
    QList<QString>getAllAccounts();
    QPair<QString, QString>getUserPassword(const QString &account);
    QString getUserAvatar(const QString &account);
};

#endif // DATABASEMANAGER_H
