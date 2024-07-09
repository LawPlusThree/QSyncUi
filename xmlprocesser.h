#ifndef XMLPROCESSER_H
#define XMLPROCESSER_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QEventLoop>
#include <QObject>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDateTime>
#include <QString>
#include <QXmlStreamWriter>
#include <QtXml>


class XmlProcesser : public QObject
{
    Q_OBJECT

public:
    explicit XmlProcesser(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void toXml(QByteArray &byteArray);
    void processXml(QXmlStreamReader &xmlReader);

private:
    // 存储文件信息列表
    QFileInfoList fileInfoList;
    QByteArray dataxml;
    //QNetworkAccessManager* manager;

public:
    // 获取文件信息列表
    QFileInfoList getFileInfoList() const { return fileInfoList; }
};

#endif // XMLPROCESSER_H
