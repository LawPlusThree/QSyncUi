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
#include <QDomDocument>
struct headHeader {
    QString ifModifiedSince;   // If-Modified-Since 请求头
    QString ifUnmodifiedSince; // If-Unmodified-Since 请求头
    QString ifMatch;           // If-Match 请求头
    QString ifNoneMatch;       // If-None-Match 请求头
};
struct preRequest{
    QMap<QString, QString> queryParams;
    QByteArray data;
    QString contentType;
    QMap<QString,QString> customHeaders;
};
struct preResponse{
    QByteArray data;
    QMap<QString,QString> headers;
    QMap<QString,QString> getMetaDatas(){
        //  x-cos-meta-*
        QMap<QString,QString> metaDatas;
        for(auto it=headers.begin();it!=headers.end();it++){
            if(it.key().startsWith("x-cos-meta-")){
                metaDatas[it.key().mid(11)]=it.value();
            }
        }
        return metaDatas;
    };
    int statusCode;
};
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
    QString listObjects(const QString &prefix, const QString &marker);
    bool putObject(const QString &path, const QByteArray &data,const QString &contentType="application/octet-stream");
    bool putLocalObject(const QString &path, const QString &localpath);
    QString initMultiUpload(const QString &path,const QMap<QString,QString> metaDatas=QMap<QString,QString>(),const  QString &contentType="application/octet-stream");
    QString initLocalMultiUpload(const QString &path, const  QString &localpath,QMap<QString,QString> metaDatas=QMap<QString,QString>());
    QString uploadPart(const QString &path, const QString &uploadId, int partNumber, const QByteArray &data);
    QString completeMultipartUpload(QString path, QString uploadId, QMap<int, QString> partEtagMap);
    QByteArray getObject(const QString &path,const QString &versionId, QMap<QString,QString> &respHeaders);
    bool save2Local(const QString &path, const QString &localpath,const QString &versionId, QMap<QString,QString> &respMetaDatas);
    preResponse headObject(const QString &path,const QString &versionId, headHeader &reqHeader);
    preResponse deleteObject(const QString &path, const QString &versionId);
    QString multiUpload(const QString &path, const QString &localpath, QMap<QString,QString> metaDatas=QMap<QString,QString>());
    bool isExist(preResponse &response);//和headobject一起使用，判断文件是否存在
    bool setManager(QNetworkAccessManager* submanager);
public slots:
    void onNewLocalPutRequest(const QString &path, const QString &localpath, int & task_id)
    {

    }
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
    {

    }
signals:
    void PutRequestFinished(int task_id, bool success);
    void TaskProgress(int task_id, int progress);
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
    QString _getContentTypeByPath(const QString &path);
    QString _getContentMD5(const QByteArray &data);
    //QString _getContentCRC64(const QString &path);//获取本地文件crc64
    QNetworkRequest buildGetRequest(const QString& path,const QMap<QString, QString> queryParams);
    QNetworkRequest buildPutRequest(const QString& path,const QMap<QString, QString> queryParams, const QByteArray& data);
    QNetworkRequest buildHeadRequest(const QString& path,const QMap<QString, QString> queryParams);
    QNetworkRequest buildDeleteRequest(const QString& path,const QMap<QString, QString> queryParams);
    QNetworkRequest buildPostRequest(const QString& path,const QMap<QString, QString> queryParams, const QByteArray& data);
    preResponse invokeGetFileRequest(const QString& path, const preRequest& request);
    preResponse invokePutRequest(const QString& path, const preRequest& request);
    preResponse invokeHeadRequest(const QString& path, const preRequest& request);
    preResponse invokeDeleteRequest(const QString& path, const preRequest& request);
    preResponse invokePostRequest(const QString& path, const preRequest& request);
    preResponse invokeGetFileRequestWithProgress(const QString& path, const preRequest& request);
    // 构建XML字符串

    QString buildTagXmlFromMap(const QMap<QString, QString> &map);

    // 从XML字符串解析为QMap
    QMap<QString, QString> parseTagXmlToMap(const QString &xmlString);
    QString buildCompleteUploadXml(QMap<int, QString>);
};
#endif // COSCLIENT_H
