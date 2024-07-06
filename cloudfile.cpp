#include "cloudfile.h"


void CloudFile::readCloudDir(const QString &path)
{
    // 创建网络访问管理器
    QNetworkAccessManager manager;

    // 创建网络请求
    QNetworkRequest request;
    QString BucketName="<BucketName-APPID>";
    QString Region="<Region>";
    QString url=QString("http://%1&.cos.%2&.myqcloud.com").arg(BucketName).arg(Region);
    request.setUrl(QUrl(url));

    // 设置请求方法为 GET
    request.setRawHeader("Authorization", "Auth String");

    // 发送请求
    QNetworkReply *reply = manager.get(request);

    // 等待请求完成
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    // 处理响应
    if (reply->error() == QNetworkReply::NoError) {
        // 读取响应数据
        dataxml = reply->readAll();
        // 在这里处理响应数据
        qDebug() << "Response:" << dataxml;
        processXml(dataxml);
    } else {
        // 处理请求错误
        qDebug() << "Request error:" << reply->errorString();
    }

    // 清理资源
    reply->deleteLater();
}

void CloudFile::processXml(QByteArray &byteArray) {
    QXmlStreamWriter xmlWriter(&byteArray);

    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("ListBucketResult");

    xmlWriter.writeStartElement("Name");
    xmlWriter.writeCharacters("string");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("EncodingType");
    xmlWriter.writeCharacters("string");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("Prefix");
    xmlWriter.writeCharacters("string");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("Marker");
    xmlWriter.writeCharacters("string");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("MaxKeys");
    xmlWriter.writeCharacters("integer");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("Delimiter");
    xmlWriter.writeCharacters("string");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("IsTruncated");
    xmlWriter.writeCharacters("boolean");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("NextMarker");
    xmlWriter.writeCharacters("string");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("CommonPrefixes");
    xmlWriter.writeStartElement("Prefix");
    xmlWriter.writeCharacters("string");
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement(); // 结束 CommonPrefixes

    xmlWriter.writeStartElement("CommonPrefixes");
    xmlWriter.writeStartElement("Prefix");
    xmlWriter.writeCharacters("string");
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement(); // 结束 CommonPrefixes

    // 添加 Contents 元素示例，可以根据需要重复添加多个 Contents 元素
    for(int i=0;i<2;i++){
        xmlWriter.writeStartElement("Contents");
        xmlWriter.writeStartElement("Key");
        xmlWriter.writeCharacters("string");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("LastModified");
        xmlWriter.writeCharacters("date");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("ETag");
        xmlWriter.writeCharacters("string");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("Size");
        xmlWriter.writeCharacters("integer");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("Owner");
        xmlWriter.writeStartElement("ID");
        xmlWriter.writeCharacters("string");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("DisplayName");
        xmlWriter.writeCharacters("string");
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement(); // 结束 Owner

        xmlWriter.writeStartElement("StorageClass");
        xmlWriter.writeCharacters("Enum");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("StorageTier");
        xmlWriter.writeCharacters("Enum");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("RestoreStatus");
        xmlWriter.writeCharacters("Enum");
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement(); // 结束 Contents
    }

    xmlWriter.writeEndElement(); // 结束 ListBucketResult
    xmlWriter.writeEndDocument();

    xmlWriter.device()->close();
}

