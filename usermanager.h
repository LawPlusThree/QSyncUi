#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QPair>

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(const QString &filePath, QObject *parent = nullptr);
    bool insertUser(const QString &account, const QString &hashedPassword);
    bool updateUserInfo(const QString &account, const QString &newHashedPassword);
    QList<QString> getAllAccounts();
    QPair<QString, QString> getUserPassword(const QString &account);

private:
    QString filePath_;
    QMap<QString, QString> userMap_;
    bool saveToFile();
    bool loadFromFile();
};
#endif // USERMANAGER_H
