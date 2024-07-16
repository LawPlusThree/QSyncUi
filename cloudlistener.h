#ifndef CLOUDLISTENER_H
#define CLOUDLISTENER_H

#include <QObject>
#include <QThread>
#include "cosclient.h"

class CloudListener:QThread
{
    Q_OBJECT
public:
    COSConfig config;
    COSClient *cosclient;
    CloudListener();
    CloudListener(COSConfig configi):config(configi){
        cosclient=new COSClient(config);
    };
    void run() override;
signals:
    void cloudDirectoryChanged(const QString &cloudPath);
};

#endif // CLOUDLISTENER_H
