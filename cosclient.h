#ifndef COSCLIENT_H
#define COSCLIENT_H
#include "qdatetime.h"
#include "signhelper.h"
#include <QString>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
class COSClient : public QObject
{
    Q_OBJECT
public:
    COSClient(QObject *parent = nullptr);
    COSClient(QString bucketName, QString appId, QString region, QString secretId, QString secretKey, QString token, QDateTime expiredTime,QObject *parent = nullptr);
    QString getBucketName() const { return bucketName; }
    QString getAppId() const { return appId; }
    QString getRegion() const { return region; }
    QString getSecretId() const { return secretId; }
    QString getSecretKey() const { return secretKey; }
    QString getEndpoint() const { return endpoint; }
    QString getGeneralApiUrl() const { return generalApiUrl; }
    QString getToken() const { return token; }
    QDateTime getExpiredTime() const { return expiredTime; }
    void setBucketName(const QString &value) { bucketName = value; }
    void setAppId(const QString &value) { appId = value; }
    void setRegion(const QString &value) { region = value; }
    void setNewSession(QString secretId, QString secretKey, QString token, QDateTime expiredTime){
        this->signHelper = new SignHelper(appId, secretId, secretKey);
        this->secretId = secretId;
        this->secretKey = secretKey;
        this->token = token;
        this->expiredTime = expiredTime;
    }
    QString listObjects(const QString &prefix, const QString &delimiter);
    bool putObject(const QString &path, const QByteArray &data);
    bool putLocalObject(const QString &path, const QString &localpath);

private:
    QString bucketName;
    QString appId;
    QString region;
    QString secretId;
    QString secretKey;
    QString endpoint;
    QString generalApiUrl;
    QString token;
    QDateTime expiredTime;
    SignHelper* signHelper;
    QNetworkAccessManager* manager;
    bool preCheckSession();
    QNetworkRequest buildGetRequest(const QString& path,const QMap<QString, QString> queryParams);
    QNetworkRequest buildPutRequest(const QString& path,const QMap<QString, QString> queryParams, const QByteArray& data);
    QNetworkRequest buildHeadRequest(const QString& path,const QMap<QString, QString> queryParams);
    QNetworkRequest buildDeleteRequest(const QString& path,const QMap<QString, QString> queryParams);
    QString invokeGetRequest(const QString& path,const QMap<QString, QString> queryParams);
    bool invokePutRequest(const QString& path,const QMap<QString, QString> queryParams, const QByteArray& data);
    bool invokeHeadRequest(const QString& path,const QMap<QString, QString> queryParams);
    bool invokeDeleteRequest(const QString& path,const QMap<QString, QString> queryParams);
};
#endif // COSCLIENT_H
