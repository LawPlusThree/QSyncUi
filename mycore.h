#ifndef MYCORE_H
#define MYCORE_H

#include <QObject>
#include <QHttpServer>
#include "user.h"
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
private:
    QHttpServer server;
    User* currentUser;
signals:
};

#endif // MYCORE_H
