#ifndef SIGNHELPER_H
#define SIGNHELPER_H

#include <QString>
#include <QNetworkRequest>
#include <QCryptographicHash>
#include <QDateTime>
#include <QUrl>
#include <QMap>
#include <QList>
#include <QByteArray>
#include <QUrlQuery>

class SignHelper {
public:
    SignHelper(const QString &appId, const QString &secretId, const QString &secretKey)
        : appId(appId), secretId(secretId), secretKey(secretKey) {}

    QString generateSignature(QNetworkRequest &request, int signDurationSeconds);

private:
    QString appId;
    QString secretId;
    QString secretKey;

    QString hmacSha1(const QString &key, const QString &message);
    QString sha1(const QString &message);
    QString urlEncode(const QString &value);
    QString getKeyTime(int signDurationSeconds);
};












#endif // SIGNHELPER_H
