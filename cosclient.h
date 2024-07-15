#ifndef COSCLIENT_H
#define COSCLIENT_H
#include "qdatetime.h"
#include "signhelper.h"
#include "tasktoken.h"
#include <QString>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QDomDocument>
struct COSConfig
{
    COSConfig(){}
    COSConfig(const COSConfig& other){
        bucketName = other.bucketName;
        appId = other.appId;
        region = other.region;
        allowPrefix = other.allowPrefix;
        taskToken = other.taskToken;
    };
    QString bucketName;
    QString appId;
    QString region;
    QString allowPrefix;
    TaskToken taskToken;
};
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
#include<QFile>
#include<QFileInfo>
#include<QDir>
class COSClient : public QObject
{
    Q_OBJECT
public:
    qint64 getFileSize(const QString &path);
    QByteArray getObject(const QString &path, const QString &versionId, QMap<QString,QString> &respHeaders, qint64 startPosition) {
        preRequest request;
        if (!versionId.isEmpty()) {
            request.customHeaders.insert("versionId", versionId);
        }
        request.customHeaders.insert("Range", QString("bytes=%1-").arg(startPosition));
        preResponse response = invokeGetFileRequest(path, request);
        respHeaders = response.headers;
        return response.data;
    }
    bool save2Local(const QString &path, const QString &localpath, const QString &versionId, QMap<QString,QString> &respMetaDatas, qint64 startPosition) {
        QMap<QString, QString> tempHeaders;
        QByteArray data = getObject(path, versionId, tempHeaders, startPosition);
        QFile file(localpath);
        // 获取文件的父文件夹，如果不存在，循环创建
        QFileInfo fileInfo(localpath);
        QDir dir = fileInfo.dir();
        if (!dir.exists()) {
            dir.mkpath(dir.absolutePath());
        }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            qDebug() << "Failed To open" << localpath;
            return false;
        }
        file.write(data);
        file.close();

        // 提取以 x-cos-meta-* 开头的元数据
        for (auto it = tempHeaders.begin(); it!= tempHeaders.end(); ++it) {
            if (it.key().startsWith("x-cos-meta-")) {
                // 去除 x-cos-meta- 前缀
                respMetaDatas.insert(it.key().mid(11), it.value());
            }
        }
        return true;
    }
    // 新的 save2Local 函数，用于从指定位置开始断点续下载
    bool save2LocalWithResume(const QString &path, const QString &localpath, const QString &versionId, QMap<QString,QString> &respMetaDatas, qint64 startPosition) {
        qint64 fileSize = getFileSize(path);
        if (fileSize == -1) {
            return false;
        }

        QFile file(localpath);
        if (file.exists() && file.size() >= fileSize) {
            qDebug() << "File already downloaded completely";
            return true;
        }

        return save2Local(path, localpath, versionId, respMetaDatas, fileSize);
    }

    COSClient(QObject *parent = nullptr);
    COSClient(COSConfig config,QObject *parent = nullptr);
    COSClient(QString bucketName, QString appId, QString region, QString allowPrefix,QString secretId, QString secretKey, QString token, QDateTime expiredTime,QObject *parent = nullptr);
    QString getBucketName() const { return bucketName; }
    QString getAppId() const { return appId; }
    QString getRegion() const { return region; }
    QString getSecretId() const { return secretId; }
    QString getSecretKey() const { return secretKey; }
    QString getEndpoint() const { return endpoint; }
    QString getGeneralApiUrl() const { return generalApiUrl; }
    QString getToken() const { return token; }
    QDateTime getExpiredTime() const { return expiredTime; }
    bool blockingPutFinishSignal=false;
    bool blockingGetFinishSignal=false;
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
    bool putObjectCopy(const QString &path, const QString &sourcePath);
    bool putLocalObject(const QString &path, const QString &localpath);
    QString initMultiUpload(const QString &path,const QMap<QString,QString> metaDatas=QMap<QString,QString>(),const  QString &contentType="application/octet-stream");
    QString initLocalMultiUpload(const QString &path, const  QString &localpath,QMap<QString,QString> metaDatas=QMap<QString,QString>());
    QString uploadPart(const QString &path, const QString &uploadId, int partNumber, const QByteArray &data);
    QString completeMultipartUpload(QString path, QString uploadId, QMap<int, QString> partEtagMap);
    QByteArray getObject(const QString &path,const QString &versionId, QMap<QString,QString> &respHeaders);
    preResponse headObject(const QString &path,const QString &versionId, headHeader &reqHeader);
    preResponse deleteObject(const QString &path, const QString &versionId);
    bool isExist(preResponse &response);//和headobject一起使用，判断文件是否存在
    bool setManager(QNetworkAccessManager* submanager);
public slots:
    bool save2LocalWithoutVersion(const QString &path, const QString &localpath);
    QString multiUpload(const QString &path, const QString &localpath, QMap<QString,QString> metaDatas=QMap<QString,QString>());
    QString ResumeMultiUpload(const QString &path, const QString &localpath, QMap<QString,QString>,QMap<int, QString>,QString);
    bool save2Local(const QString &path, const QString &localpath,const QString &versionId, QMap<QString,QString> &respMetaDatas);
    void onNewLocalPutRequest(const QString &path, const QString &localpath, int & task_id)
    {

    }
    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
    {

    }
signals:
    void PutRequestFinished(int task_id, bool success);
    void TaskProgress(int task_id, int progress);
    void UploadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void DownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void progress(qint64 bytesReceived, qint64 bytesTotal);
    void finished(QNetworkReply::NetworkError error);
private:
    QString bucketName;
    QString appId;
    QString region;
    QString secretId;
    QString secretKey;
    QString endpoint;
    QString allowPrefix;
    QString generalApiUrl;
    QString token;
    QDateTime expiredTime;
    SignHelper* signHelper;
    QNetworkAccessManager* manager;
    bool preCheckSession();
    QString _getContentTypeByPath(const QString &path);
    QString _getContentMD5(const QByteArray &data);
    QString _prefixHandle(const QString &rawPath);
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
