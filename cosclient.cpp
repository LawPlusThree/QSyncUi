#include "cosclient.h"
#include "qeventloop.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QFile>
COSClient::COSClient(QObject *parent)
{
    return ;
}

COSClient::COSClient(QString bucketName, QString appId, QString region, QString secretId, QString secretKey, QString token, QDateTime expiredTime, QObject *parent)
{
    this->bucketName = bucketName;
    this->appId = appId;
    this->region = region;
    this->secretId = secretId;
    this->secretKey = secretKey;
    this->token = token;
    this->expiredTime = expiredTime;
    this->endpoint = bucketName + "-" + appId + ".cos." + region + ".myqcloud.com";
    this->generalApiUrl = "https://" + endpoint;
    this->signHelper = new SignHelper(appId, secretId, secretKey);
    manager = new QNetworkAccessManager(this);
}

QString COSClient::listObjects(const QString &prefix, const QString &delimiter)
{
    QMap<QString, QString> queryParams;
    queryParams.insert("prefix", prefix);
    queryParams.insert("delimiter", delimiter);
    return invokeGetRequest("/", queryParams);
}

bool COSClient::putObject(const QString &path, const QByteArray &data)
{
    QMap<QString, QString> queryParams;
    return invokePutRequest(path, queryParams, data);
}

bool COSClient::putLocalObject(const QString &path, const QString &localpath)
{
    QFile file(localpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QByteArray data = file.readAll();
    file.close();
    return putObject(path, data);
}

QString COSClient::invokeGetRequest(const QString &path, const QMap<QString, QString> queryParams)
{
    QNetworkRequest request = buildGetRequest(path, queryParams);
    qDebug()<<request.url();
    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->readAll();
}

bool COSClient::invokePutRequest(const QString &path, const QMap<QString, QString> queryParams, const QByteArray &data)
{
    QNetworkRequest request = buildPutRequest(path, queryParams,data);
    QNetworkReply *reply = manager->put(request, data);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->error()==QNetworkReply::NoError;
}

bool COSClient::invokeHeadRequest(const QString &path, const QMap<QString, QString> queryParams)
{
    QNetworkRequest request = buildHeadRequest(path, queryParams);
    QNetworkReply *reply = manager->head(request);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->error()==QNetworkReply::NoError;
}

bool COSClient::invokeDeleteRequest(const QString &path, const QMap<QString, QString> queryParams)
{
    QNetworkRequest request = buildDeleteRequest(path, queryParams);
    QNetworkReply *reply = manager->deleteResource(request);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->error()==QNetworkReply::NoError;
}

bool COSClient::preCheckSession()
{
    if (token == "")
    {
        return true;
    }
    if (expiredTime < QDateTime::currentDateTime())
    {
        qDebug() << expiredTime;
        qDebug() << QDateTime::currentDateTime();
        return false;
    }
    return true;
}

QNetworkRequest COSClient::buildGetRequest(const QString &path, const QMap<QString, QString> queryParams)
{
    if (!preCheckSession())
    {
        return QNetworkRequest();
    }
    QUrl url;
    QUrlQuery query;
    url=QUrl(generalApiUrl+path);
    for(auto it=queryParams.begin();it!=queryParams.end();it++)
    {
        query.addQueryItem(it.key(),it.value());
    }
    qDebug()<<url;
    url.setQuery(query);
    qDebug()<<url;
    QNetworkRequest request(url);
    qDebug()<<request.url();
    request.setRawHeader("Host",endpoint.toUtf8());
    //设置请求方法
    request.setAttribute(QNetworkRequest::CustomVerbAttribute,"GET");
    QString sig=this->signHelper->generateSignature(request,300);
    request.setRawHeader("Authorization",sig.toUtf8());
    if(token!="")
    {
        request.setRawHeader("x-cos-security-token",token.toUtf8());
    }
    return request;

}

QNetworkRequest COSClient::buildPutRequest(const QString &path, const QMap<QString, QString> queryParams, const QByteArray &data)
{
    if (!preCheckSession())
    {
        return QNetworkRequest();
    }
    QUrl url;
    QUrlQuery query;
    url=QUrl(generalApiUrl+path);
    for(auto it=queryParams.begin();it!=queryParams.end();it++)
    {
        query.addQueryItem(it.key(),it.value());
    }
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("Host",endpoint.toUtf8());
    //设置请求方法
    request.setAttribute(QNetworkRequest::CustomVerbAttribute,"PUT");
    QString sig=this->signHelper->generateSignature(request,300);
    request.setRawHeader("Authorization",sig.toUtf8());
    if(token!="")
    {
        request.setRawHeader("x-cos-security-token",token.toUtf8());
    }
    return request;
}

QNetworkRequest COSClient::buildHeadRequest(const QString &path, const QMap<QString, QString> queryParams)
{
    if (!preCheckSession())
    {
        return QNetworkRequest();
    }
    QUrl url;
    QUrlQuery query;
    url=QUrl(generalApiUrl+path);
    for(auto it=queryParams.begin();it!=queryParams.end();it++)
    {
        query.addQueryItem(it.key(),it.value());
    }
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("Host",endpoint.toUtf8());
    //设置请求方法
    request.setAttribute(QNetworkRequest::CustomVerbAttribute,"HEAD");
    QString sig=this->signHelper->generateSignature(request,300);
    request.setRawHeader("Authorization",sig.toUtf8());
    if(token!="")
    {
        request.setRawHeader("x-cos-security-token",token.toUtf8());
    }
    return request;
}

QNetworkRequest COSClient::buildDeleteRequest(const QString &path, const QMap<QString, QString> queryParams)
{
    if (!preCheckSession())
    {
        return QNetworkRequest();
    }
    QUrl url;
    QUrlQuery query;
    url=QUrl(generalApiUrl+path);
    for(auto it=queryParams.begin();it!=queryParams.end();it++)
    {
        query.addQueryItem(it.key(),it.value());
    }
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setRawHeader("Host",endpoint.toUtf8());
    //设置请求方法
    request.setAttribute(QNetworkRequest::CustomVerbAttribute,"DELETE");
    QString sig=this->signHelper->generateSignature(request,300);
    request.setRawHeader("Authorization",sig.toUtf8());
    if(token!="")
    {
        request.setRawHeader("x-cos-security-token",token.toUtf8());
    }
    return request;
}
