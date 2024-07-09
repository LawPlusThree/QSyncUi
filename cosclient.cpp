#include "cosclient.h"
#include "qeventloop.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
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

bool COSClient::putObject(const QString &path, const QByteArray &data,const QString &contentType)
{
    QMap<QString, QString> queryParams;
    return invokePutRequest(path, queryParams, data,contentType);
}

bool COSClient::putLocalObject(const QString &path, const QString &localpath)
{
    QFile file(localpath);
    QString contentType = _getContentTypeByPath(localpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    QByteArray data = file.readAll();
    file.close();
    return putObject(path, data, contentType);
}

QByteArray COSClient::getObject(const QString &path,const QString &versionId)
{
    QMap<QString, QString> queryParams;
    if(versionId!="")
    {
        queryParams.insert("versionId",versionId);
    }
    return invokeGetFileRequest(path,queryParams);
}

bool COSClient::save2Local(const QString &path, const QString &localpath,const QString &versionId)
{
    QFile file(localpath);
    //如果文件不存在则创建文件
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(getObject(path,versionId));
        } else {
            qDebug() << "无法创建文件：" << localpath;
            return false;
        }
    }
    file.close();
    return true;
}

QByteArray COSClient::invokeGetFileRequest(const QString &path, const QMap<QString, QString> queryParams)
{
    QNetworkRequest request = buildGetRequest(path, queryParams);
    qDebug()<<request.url();
    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->readAll();
}

QByteArray COSClient::getObject(const QString &path,const QString &versionId)
{
    QMap<QString, QString> queryParams;
    if(versionId!="")
    {
        queryParams.insert("versionId",versionId);
    }
    return invokeGetFileRequest(path,queryParams);
}

bool COSClient::save2Local(const QString &path, const QString &localpath,const QString &versionId)
{
    QFile file(localpath);
    //如果文件不存在则创建文件
    if (!file.exists()) {
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(getObject(path,versionId));
        } else {
            qDebug() << "无法创建文件：" << localpath;
            return false;
        }
    }
    file.close();
    return true;
}

QByteArray COSClient::invokeGetFileRequest(const QString &path, const QMap<QString, QString> queryParams)
{
    QNetworkRequest request = buildGetRequest(path, queryParams);
    qDebug()<<request.url();
    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply->readAll();
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

bool COSClient::invokePutRequest(const QString &path, const QMap<QString, QString> queryParams, const QByteArray &data,QString contentType)
{
    QNetworkRequest request = buildPutRequest(path, queryParams,data);
    request.setHeader(QNetworkRequest::ContentTypeHeader,contentType);
    request.setRawHeader("Content-MD5",_getContentMD5(data).toUtf8());
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

QString COSClient::buildTagXmlFromMap(const QMap<QString, QString> &map) {
    QDomDocument doc;
    QDomElement root = doc.createElement("Tagging");
    doc.appendChild(root);
    QDomElement tagSet = doc.createElement("TagSet");
    root.appendChild(tagSet);

    QMapIterator<QString, QString> i(map);
    while (i.hasNext()) {
        i.next();
        QDomElement tag = doc.createElement("Tag");
        QDomElement key = doc.createElement("Key");
        QDomElement value = doc.createElement("Value");
        QDomText keyText = doc.createTextNode(i.key());
        QDomText valueText = doc.createTextNode(i.value());
        key.appendChild(keyText);
        value.appendChild(valueText);
        tag.appendChild(key);
        tag.appendChild(value);
        tagSet.appendChild(tag);
    }

    return doc.toString();
}

QMap<QString, QString> COSClient::parseTagXmlToMap(const QString &xmlString) {
    QMap<QString, QString> map;
    QDomDocument doc;
    doc.setContent(xmlString);
    QDomNodeList tags = doc.elementsByTagName("Tag");

    for (int i = 0; i < tags.count(); ++i) {
        QDomNode tagNode = tags.at(i);
        if (tagNode.isElement()) {
            QDomElement tagElement = tagNode.toElement();
            QString key = tagElement.elementsByTagName("Key").at(0).toElement().text();
            QString value = tagElement.elementsByTagName("Value").at(0).toElement().text();
            map.insert(key, value);
        }
    }

    return map;
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

QString COSClient::_getContentTypeByPath(const QString &path)
{
    QFileInfo fileInfo(path);
    if (fileInfo.suffix() == "jpg"||fileInfo.suffix() == "jpeg")
    {
        return "image/jpeg";
    }
    else if (fileInfo.suffix() == "png")
    {
        return "image/png";
    }
    else if (fileInfo.suffix() == "gif")
    {
        return "image/gif";
    }
    else if (fileInfo.suffix() == "bmp")
    {
        return "image/bmp";
    }
    else if (fileInfo.suffix() == "txt")
    {
        return "text/plain";
    }
    else if (fileInfo.suffix() == "html")
    {
        return "text/html";
    }
    else if (fileInfo.suffix() == "xml")
    {
        return "text/xml";
    }
    else if (fileInfo.suffix() == "pdf")
    {
        return "application/pdf";
    }
    else if (fileInfo.suffix() == "doc" || fileInfo.suffix() == "docx")
    {
        return "application/msword";
    }
    else if (fileInfo.suffix() == "xls" || fileInfo.suffix() == "xlsx")
    {
        return "application/vnd.ms-excel";
    }
    else if (fileInfo.suffix() == "ppt" || fileInfo.suffix() == "pptx")
    {
        return "application/vnd.ms-powerpoint";
    }
    else if (fileInfo.suffix() == "zip")
    {
        return "application/zip";
    }
    else if (fileInfo.suffix() == "rar")
    {
        return "application/x-rar-compressed";
    }
    else if (fileInfo.suffix() == "tar")
    {
        return "application/x-tar";
    }
    else if (fileInfo.suffix() == "gz")
    {
        return "application/x-gzip";
    }
    else if (fileInfo.suffix() == "7z")
    {
        return "application/x-7z-compressed";
    }
    else if (fileInfo.suffix() == "mp3")
    {
        return "audio/mpeg";
    }
    else if (fileInfo.suffix() == "wav")
    {
        return "audio/wav";
    }
    else if (fileInfo.suffix() == "mp4")
    {
        return "video/mp4";
    }
    else if (fileInfo.suffix() == "avi")
    {
        return "video/avi";
    }
    else if (fileInfo.suffix() == "mkv")
    {
        return "video/x-matroska";
    }
    else if (fileInfo.suffix() == "flv")
    {
        return "video/x-flv";
    }
    else if (fileInfo.suffix() == "mov")
    {
        return "video/quicktime";
    }
    else
    {
        return "application/octet-stream";
    }
}

QString COSClient::_getContentMD5(const QByteArray &data)
{
    //RFC 1864中定义的请求体内容的16字节二进制 MD5 哈希值的 Base64 编码形式
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    QByteArray base64 = hash.toBase64();
    return base64;
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

