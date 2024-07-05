#include "user.h"

User::User(QObject *parent)
    : QObject(parent)
{}

User::User(const QString &username, const QString &account, const QString &password, QObject *parent)
    : QObject(parent)
    , username(username)
    , account(account)
    , hashedPassword(password)
{
    apiRequest=new ApiRequest(this);
    apiRequest->setBaseUrl("https://syncapi.snakekiss.com");
}

User::User(const QString &account, const QString &password, QObject *parent)
    : QObject(parent)
    , account(account)
    , hashedPassword(password)
{
    apiRequest=new ApiRequest(this);
    apiRequest->setBaseUrl("https://syncapi.snakekiss.com");
}

void User::enroll()
{
    QString postData
    = QString("username=%1&email=%2&password=%3").arg(username).arg(account).arg(hashedPassword);
    apiRequest->post("/register",postData.toUtf8());
}

void User::login()
{
    QString postData = QString("email=%1&password=%2").arg(account).arg(hashedPassword);
    apiRequest->post("/login",postData.toUtf8());
}

void User::forgetPassword()
{
    //找回密码功能
}

QString User::getUserHash() const
{
    QByteArray hash = QCryptographicHash::hash(account.toUtf8(), QCryptographicHash::Sha1);
    return hash.toHex();
}

QString User::getSession()
{
    return session;
}





