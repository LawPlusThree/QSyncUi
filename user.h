#ifndef USER_H
#define USER_H
#include <QString>
#include <QCryptographicHash>


class User
{
private:
    QString account;
    QString token="222";
    QString hashedPassword;
public:
    User();
    User(QString account, QString password) {
        this->account = account;
        this->hashedPassword = password;
    }
    QString login(QString password) {
        //https post a requests
        return token;
    }
    QString getUserHash() {
        QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
        return hash.toHex(); // 将散列值转换为十六进制字符串并返回
    }
};

#endif // USER_H
