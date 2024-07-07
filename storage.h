#ifndef STORAGE_H
#define STORAGE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class Storage : public QObject
{
    Q_OBJECT
public:
    explicit Storage(const QByteArray &c);

public:
    QNetworkAccessManager *minio;
    QString bucket;
    QString localPrefix;
    QString remotePrefix;
    bool symLink;

signals:
};

#endif // STORAGE_H

