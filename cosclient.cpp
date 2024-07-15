#include "cosclient.h"
#include "qeventloop.h"
#include <QUrlQuery>
#include <QNetworkReply>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include "bucket.h"
#include "xmlprocesser.h"

COSClient::COSClient(QObject *parent)
{
    return ;
}



COSClient::COSClient(QString bucketName, QString appId, QString region,QString allowPrefix, QString secretId, QString secretKey, QString token, QDateTime expiredTime, QObject *parent)
{
    this->bucketName = bucketName;
    this->appId = appId;
    this->region = region;
    this->allowPrefix = allowPrefix;
    this->secretId = secretId;
    this->secretKey = secretKey;
    this->token = token;
    this->expiredTime = expiredTime;
    this->endpoint = bucketName + "-" + appId + ".cos." + region + ".myqcloud.com";
    this->generalApiUrl = "https://" + endpoint;
    this->signHelper = new SignHelper(appId, secretId, secretKey);
    manager = new QNetworkAccessManager(this);
}

COSClient::COSClient(COSConfig config, QObject *parent)
    : COSClient(config.bucketName, config.appId, config.region,config.allowPrefix,
                config.taskToken.tmpSecretId, config.taskToken.tmpSecretKey,
                config.taskToken.sessionToken, config.taskToken.expiredTime, parent)
{
    // 其他初始化代码（如果有的话）
}
QString COSClient::listObjects(const QString &prefix, const QString &marker)
{
    preRequest request;
    QString prefix_=_prefixHandle(prefix);
    request.queryParams.insert("prefix", prefix_);
    request.queryParams.insert("marker", marker);
    preResponse response = invokeGetFileRequest("/", request);
    return QString::fromUtf8(response.data);
}

QString COSClient::listVersions(const QString &prefix, const QString &keyMarker, const QString &versionIdMarker, int maxKeys)
{
    preRequest request;
    QString prefix_=_prefixHandle(prefix);
    request.queryParams.insert("prefix", prefix_);
    if(keyMarker!="")
    {
        QString keyMarker_=_prefixHandle(keyMarker);
        request.queryParams.insert("key-marker", keyMarker_);
    }
    request.queryParams.insert("version-id-marker", versionIdMarker);
    request.queryParams.insert("max-keys", QString::number(maxKeys));
    request.queryParams.insert("versions", "");
    preResponse response = invokeGetFileRequest("/", request);
    return QString::fromUtf8(response.data);
}

QVector<Version> COSClient::listAllVersionsByPrefix(const QString & key)
{

    QString versionMarker = "";
    QString keyMarker = "";
    VersionResult versionResult;
    QVector<Version> res;
    do
    {
        QString xml = listVersions(key,keyMarker,versionMarker,1000);
        HistoryXMLProcesser processer;
        versionResult = processer.processXml(xml);
        for (Version version : versionResult.versions)
        {
                res.push_back(version);
        }
        versionMarker = versionResult.nextVersionIdMarker;
        keyMarker = versionResult.nextKeyMarker;
    } while (versionResult.isTruncated);
    return res;
}




QString COSClient::_prefixHandle(const QString &rawPath)
{
    if (rawPath=="/")
    {
        return "/";
    }
    QString myPath = rawPath;
    //如果前33位是allowPrefix，什么都不做
    if (myPath.startsWith(allowPrefix))
    {
        return myPath;
    }
    else
    {
        if (myPath.startsWith("/"))
        {
            myPath = myPath.mid(1);
        }
        myPath = allowPrefix + myPath;
        return myPath;
    }
}
bool COSClient::putObject(const QString &path, const QByteArray &data, const QString &contentType)
{
    preRequest request;
    request.data = data;
    request.contentType = contentType;
    preResponse response = invokePutRequest(path, request);
    return response.statusCode >= 200 && response.statusCode < 300; // Assuming success is 2xx status code
}

bool COSClient::putObjectCopy(const QString &path, const QString &sourcePath)
{
    //复制错误会返回200ok，响应体中错误判断没写，待完善
    preRequest request;
    QString sourcePath_=_prefixHandle(sourcePath);
    QString rawPath=endpoint+"/"+sourcePath_;
    request.customHeaders.insert("x-cos-copy-source", rawPath);
    preResponse response = invokePutRequest(path, request);
    return response.statusCode >= 200 && response.statusCode < 300; // Assuming success is 2xx status code
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

    QString contentType = _getContentTypeByPath(localpath); // Assuming this function exists and returns a content type based on the file extension
    return putObject(path, data, contentType);
}

QString COSClient::initMultiUpload(const QString &path, QMap<QString, QString> metaDatas,const  QString &contentType)
{
    preRequest request;
    //build metadata
    QMap<QString, QString> headers;
    for(auto it = metaDatas.begin(); it != metaDatas.end(); ++it)
    {
        headers.insert("x-cos-meta-" + it.key(), it.value());
    }
    request.customHeaders = headers;
    request.contentType = contentType;
    request.queryParams.insert("uploads", "");
    preResponse response = invokePostRequest(path, request);
    //解析xml：InitiateMultipartUploadResult->UploadId
    QDomDocument doc;
    doc.setContent(response.data);
    QDomElement root = doc.documentElement();
    QDomElement uploadIdElement = root.firstChildElement("UploadId");
    return uploadIdElement.text();
}

QString COSClient::initLocalMultiUpload(const QString &path, const QString &localpath, QMap<QString, QString> metaDatas)
{
    QString contentType = _getContentTypeByPath(localpath); // Assuming this function exists and returns a content type based on the file extension
    return initMultiUpload(path, metaDatas, contentType);
}

QString COSClient::uploadPart(const QString &path, const QString &uploadId, int partNumber, const QByteArray &data)
{
    preRequest request;
    request.data = data;
    request.contentType = "application/octet-stream";
    request.queryParams.insert("partNumber", QString::number(partNumber));
    request.queryParams.insert("uploadId", uploadId);
    preResponse response = invokePutRequest(path, request);
    return response.headers.value("ETag");
}

QString COSClient::completeMultipartUpload(QString path, QString uploadId, QMap<int, QString> partEtagMap)
{
    preRequest request;
    request.data = buildCompleteUploadXml(partEtagMap).toUtf8();
    request.contentType = "application/xml";
    request.queryParams.insert("uploadId", uploadId);
    preResponse response = invokePostRequest(path, request);
    //xml, wait for processing
    return QString::fromUtf8(response.data);
}

qint64 COSClient::getFileSize(const QString &path) {
    QNetworkRequest request = buildGetRequest(path, {});
    request.setRawHeader("Range", "bytes=0-0");
    QNetworkReply* reply = manager->head(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error()!= QNetworkReply::NoError) {
        qDebug() << "Error getting file size: " << reply->errorString();
        reply->deleteLater();
        return -1;
    }

    qint64 fileSize = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
    reply->deleteLater();
    return fileSize;
}

QByteArray COSClient::getObject(const QString &path, const QString &versionId, QMap<QString,QString> &respHeaders)
{
    preRequest request;
    if(!versionId.isEmpty())
    {
        request.customHeaders.insert("versionId", versionId);
    }
    preResponse response = invokeGetFileRequest(path, request);
    respHeaders = response.headers; // Assuming we want to pass back the headers
    return response.data;
}

bool COSClient::save2Local(const QString &path, const QString &localpath, const QString &versionId, QMap<QString,QString> &respMetaDatas)
{
    QMap<QString, QString> tempHeaders;
    QByteArray data = getObject(path, versionId, tempHeaders);
    QFile file(localpath);
    //获取文件的父文件夹，如果不存在，循环创建
    QFileInfo fileInfo(localpath);
    QDir dir = fileInfo.dir();
    if (!dir.exists())
    {
        dir.mkpath(dir.absolutePath());
    }

    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed To create" << localpath;
        return false;
    }
    file.write(data);
    file.close();

    // Extracting meta-data starting with x-cos-meta-*
    for(auto it = tempHeaders.begin(); it != tempHeaders.end(); ++it)
    {
        if(it.key().startsWith("x-cos-meta-"))
        {
            // Don't keep the x-cos-meta- prefix
            respMetaDatas.insert(it.key().mid(11), it.value());
        }
    }
    return true;
}

bool COSClient::save2LocalWithoutVersion(const QString &path, const QString &localpath)
{
    QFile file(localpath);
    int startPosition;
    if (file.exists()) {
        startPosition = file.size();
    } else {
        startPosition = 0; // 文件不存在，从0开始下载
    }
    QMap<QString, QString> tempMetaDatas;
    return save2Local(path, localpath, "", tempMetaDatas);
    //return save2LocalWithResume(path,localpath,"",tempMetaDatas,startPosition);
}

preResponse COSClient::headObject(const QString &path,const QString &versionId, headHeader &reqHeader)
{
    preRequest request;
    if(!versionId.isEmpty()) {
        request.customHeaders.insert("versionId", versionId);
    }
    if(!reqHeader.ifModifiedSince.isEmpty()) {
        request.customHeaders.insert("If-Modified-Since", reqHeader.ifModifiedSince);
    }
    if(!reqHeader.ifUnmodifiedSince.isEmpty()) {
        request.customHeaders.insert("If-Unmodified-Since", reqHeader.ifUnmodifiedSince);
    }
    if(!reqHeader.ifMatch.isEmpty()) {
        request.customHeaders.insert("If-Match", reqHeader.ifMatch);
    }
    if(!reqHeader.ifNoneMatch.isEmpty()) {
        request.customHeaders.insert("If-None-Match", reqHeader.ifNoneMatch);
    }
    preResponse response = invokeHeadRequest(path, request);

    return response;
}

preResponse COSClient::deleteObject(const QString &path, const QString &versionId)
{
    preRequest request;
    bool haveId=!versionId.isEmpty();
    if(haveId) {
        request.customHeaders.insert("versionId", versionId);
    }
    preResponse response = invokeDeleteRequest(path,request);
    // 获取 x-cos-version-id 响应头
    QString responseVersionId = response.headers.value("x-cos-version-id");
    // 获取 x-cos-delete-marker 响应头
    bool deleteMarker = response.headers.value("x-cos-delete-marker")=="true"?"true":"false";
    if(haveId&&deleteMarker){
        qDebug()<<"Mark "<<responseVersionId<<" to deleted";
    }
    else if(!haveId && deleteMarker){
        qDebug()<<"Delete "<<path<<" as latest version";
    }
    return response;
}

QString COSClient::multiUpload(const QString &path, const QString &localpath, QMap<QString, QString> metaDatas)
{
    //判断文件存在且可读
    QFile file(localpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return "";
    }
    //初始化分片上传
    QString uploadId = initLocalMultiUpload(path, localpath, metaDatas);
    //分片上传
    int partNumber = 1;
    QMap<int, QString> partEtagMap;
    while (!file.atEnd())
    {
        QByteArray data = file.read(2 * 1024 * 1024); // Assuming 2MB parts
        QString etag = uploadPart(path, uploadId, partNumber, data);
        emit progress(file.pos(), file.size());
        partEtagMap.insert(partNumber, etag);
        partNumber++;
    }
    //完成分片上传
    QString result = completeMultipartUpload(path, uploadId, partEtagMap);
    file.close();
    return result;
}

QString COSClient::ResumeMultiUpload(const QString &path, const QString &localpath, QMap<QString, QString> metaDatas, QMap<int, QString> partEtagMap,QString uploadId)
{
    //判断文件存在且可读
    QFile file(localpath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return "";
    }
    //分片上传
    int partNumber = partEtagMap.size() + 1;
    //从上次上传的地方开始读取
    file.seek(file.pos() + 2 * 1024 * 1024 * (partNumber - 1));
    while (!file.atEnd())
    {
        QByteArray data = file.read(2*1024*1024); // Assuming 2MB parts
        QString etag = uploadPart(path, uploadId, partNumber, data);
        emit progress(file.pos(), file.size());
        partEtagMap.insert(partNumber, etag);
        partNumber++;
    }
    //完成分片上传
    QString result = completeMultipartUpload(path, uploadId, partEtagMap);
    file.close();
    return result;
}

bool COSClient::isExist(preResponse &response)
{
    return response.statusCode!=404;
}

bool COSClient::setManager(QNetworkAccessManager *submanager)
{
    manager=submanager;
    return true;
}

// 修改后的函数实现

preResponse COSClient::invokeGetFileRequest(const QString& path, const preRequest& request) {
    preResponse response;
    // 构建请求
    QNetworkRequest networkRequest = buildGetRequest(path, request.queryParams); // 假设已经有一个构建GET请求的函数
    // 发送请求并等待响应
    for (auto it = request.customHeaders.begin(); it != request.customHeaders.end(); it++) {
        networkRequest.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    QNetworkReply* reply = manager->get(networkRequest);
    // 等待响应完成
    connect(reply, &QNetworkReply::downloadProgress, this, [this](qint64 bytesReceived, qint64 bytesTotal) {
        emit DownloadProgress(bytesReceived, bytesTotal);
        emit progress(bytesReceived, bytesTotal);
    });

    // 等待响应完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 处理响应
    response.data = reply->readAll();
    response.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QList<QByteArray> rawHeaderList = reply->rawHeaderList();
    for (const QByteArray& header : rawHeaderList) {
        response.headers.insert(QString(header), QString(reply->rawHeader(header)));
    }
    finished(reply->error());
    reply->deleteLater();
    return response;
}

preResponse COSClient::invokeGetFileRequestWithProgress(const QString& path, const preRequest& request) {
    preResponse response;
    // 构建请求
    QNetworkRequest networkRequest = buildGetRequest(path, request.queryParams);
    for (auto it = request.customHeaders.begin(); it != request.customHeaders.end(); it++) {
        networkRequest.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    // 发送请求并等待响应
    QNetworkReply* reply = manager->get(networkRequest);
    // 连接进度信号
    connect(reply, &QNetworkReply::downloadProgress, this, &COSClient::onDownloadProgress);
    // 等待响应完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    // 处理响应
    response.data = reply->readAll();
    response.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QList<QByteArray> rawHeaderList = reply->rawHeaderList();
    for (const QByteArray& header : rawHeaderList) {
        response.headers.insert(QString(header), QString(reply->rawHeader(header)));
    }
    finished(reply->error());
    reply->deleteLater();
    return response;
}

preResponse COSClient::invokePutRequest(const QString& path, const preRequest& request) {
    preResponse response;
    // 构建请求
    QNetworkRequest networkRequest = buildPutRequest(path, request.queryParams, request.data); // 使用提供的代码片段构建PUT请求
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, request.contentType);
    networkRequest.setRawHeader("Content-MD5",_getContentMD5(request.data).toUtf8());
    for (auto it = request.customHeaders.begin(); it != request.customHeaders.end(); it++) {
        networkRequest.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    // 发送请求并等待响应
    QNetworkReply* reply = manager->put(networkRequest, request.data);
    // 等待响应完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    // 处理响应
    response.data = reply->readAll();
    response.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QList<QByteArray> rawHeaderList = reply->rawHeaderList();
    for (const QByteArray& header : rawHeaderList) {
        response.headers.insert(QString(header), QString(reply->rawHeader(header)));
    }
    if(!blockingPutFinishSignal){
    emit finished(reply->error());
    }
    reply->deleteLater();
    return response;
}

preResponse COSClient::invokeHeadRequest(const QString& path, const preRequest& request) {
    preResponse response;
    // 构建请求
    QNetworkRequest networkRequest = buildHeadRequest(path, request.queryParams); // 假设已经有一个构建HEAD请求的函数
    for (auto it = request.customHeaders.begin(); it != request.customHeaders.end(); it++) {
        networkRequest.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    // 发送请求并等待响应
    QNetworkReply* reply = manager->head(networkRequest);
    // 等待响应完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    // 处理响应
    response.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QList<QByteArray> rawHeaderList = reply->rawHeaderList();
    for (const QByteArray& header : rawHeaderList) {
        response.headers.insert(QString(header), QString(reply->rawHeader(header)));
    }
    reply->deleteLater();
    return response;
}

preResponse COSClient::invokeDeleteRequest(const QString& path, const preRequest& request) {
    preResponse response;
    // 构建请求
    QNetworkRequest networkRequest = buildDeleteRequest(path, request.queryParams); // 使用提供的代码片段构建DELETE请求
    for (auto it = request.customHeaders.begin(); it != request.customHeaders.end(); it++) {
        networkRequest.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }
    QNetworkReply* reply = manager->deleteResource(networkRequest);
    // 等待响应完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    // 处理响应
    response.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QList<QByteArray> rawHeaderList = reply->rawHeaderList();
    for (const QByteArray& header : rawHeaderList) {
        response.headers.insert(QString(header), QString(reply->rawHeader(header)));
    }
    reply->deleteLater();
    return response;
}

preResponse COSClient::invokePostRequest(const QString &path, const preRequest &request)
{
    preResponse response;
    // 构建请求
    QNetworkRequest networkRequest = buildPostRequest(path, request.queryParams, request.data); // 使用提供的代码片段构建POST请求
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, request.contentType);
    networkRequest.setRawHeader("Content-MD5",_getContentMD5(request.data).toUtf8());
    // 发送请求并等待响应
    QNetworkReply* reply = manager->post(networkRequest, request.data);
    // 等待响应完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    // 处理响应
    response.data = reply->readAll();
    response.statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QList<QByteArray> rawHeaderList = reply->rawHeaderList();
    for (const QByteArray& header : rawHeaderList) {
        response.headers.insert(QString(header), QString(reply->rawHeader(header)));
    }
    //if is cos mulit upload response, SEND signal
    if(reply->url().toString().contains("uploadId"))
    {
       emit finished(reply->error());
    }
    reply->deleteLater();
    return response;
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

QString COSClient::buildCompleteUploadXml(QMap<int, QString>partEtagMap)
{
/*
  <CompleteMultipartUpload>
    <Part>
        <PartNumber>integer</PartNumber>
        <ETag>string</ETag>
    </Part>
    <Part>
        <PartNumber>integer</PartNumber>
        <ETag>string</ETag>
    </Part>
</CompleteMultipartUpload>
*/
    QDomDocument doc;
    QDomElement root = doc.createElement("CompleteMultipartUpload");
    doc.appendChild(root);

    QMapIterator<int, QString> i(partEtagMap);
    while (i.hasNext()) {
        i.next();
        QDomElement part = doc.createElement("Part");
        QDomElement partNumber = doc.createElement("PartNumber");
        QDomElement eTag = doc.createElement("ETag");
        QDomText partNumberText = doc.createTextNode(QString::number(i.key()));
        QDomText eTagText = doc.createTextNode(i.value());
        partNumber.appendChild(partNumberText);
        eTag.appendChild(eTagText);
        part.appendChild(partNumber);
        part.appendChild(eTag);
        root.appendChild(part);
    }

    return doc.toString();
}

bool COSClient::preCheckSession()
{
    if (token == "")
    {
        return true;
    }
    if (expiredTime < QDateTime::currentDateTime())
    {
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
    QString realPath=path;
    realPath=_prefixHandle(realPath);
    if(!path.startsWith("/")){
        realPath.prepend("/");
    }
    url=QUrl(generalApiUrl+realPath);
    for(auto it=queryParams.begin();it!=queryParams.end();it++)
    {
        query.addQueryItem(it.key(),it.value());
    }
    url.setQuery(query);
    QNetworkRequest request(url);
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
    QString realPath=path;
    realPath=_prefixHandle(realPath);
    if(!path.startsWith("/")){
        realPath.prepend("/");
    }
    url=QUrl(generalApiUrl+realPath);
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
    QString realPath=path;
    realPath=_prefixHandle(realPath);
    if(!path.startsWith("/")){
        realPath.prepend("/");
    }
    url=QUrl(generalApiUrl+realPath);
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
    QString realPath=path;
    realPath=_prefixHandle(realPath);
    if(!path.startsWith("/")){
        realPath.prepend("/");
    }
    url=QUrl(generalApiUrl+realPath);
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

QNetworkRequest COSClient::buildPostRequest(const QString &path, const QMap<QString, QString> queryParams, const QByteArray &data)
{
    if (!preCheckSession())
    {
        return QNetworkRequest();
    }
    QUrl url;
    qDebug()<<queryParams;
    QUrlQuery query;
    QString realPath=path;
    realPath=_prefixHandle(realPath);
    if(!path.startsWith("/")){
        realPath.prepend("/");
    }
    url=QUrl(generalApiUrl+realPath);
    for(auto it=queryParams.begin();it!=queryParams.end();it++)
    {
        query.addQueryItem(it.key(),it.value());
    }
    url.setQuery(query);
    QNetworkRequest request(url);
    qDebug()<<request.url();
    request.setRawHeader("Host",endpoint.toUtf8());
    //设置请求方法
    request.setAttribute(QNetworkRequest::CustomVerbAttribute,"POST");
    QString sig=this->signHelper->generateSignature(request,300);
    request.setRawHeader("Authorization",sig.toUtf8());
    if(token!="")
    {
        request.setRawHeader("x-cos-security-token",token.toUtf8());
    }
    return request;
}


