#ifndef CLOUDFILE_H
#define CLOUDFILE_H

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


class CloudFile : public QObject
{
    Q_OBJECT

public:
    explicit CloudFile(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    void readCloudDir(const QString &path);// 读取云端文件夹,返回xml
    void processXml(QByteArray &byteArray);;
private:
    // 存储文件信息列表
    QFileInfoList fileInfoList;
    QByteArray dataxml;
    //QNetworkAccessManager* manager;

public:
    // 获取文件信息列表
    QFileInfoList getFileInfoList() const { return fileInfoList; }
};

#endif // CLOUDFILE_H
