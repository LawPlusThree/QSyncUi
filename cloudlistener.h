#ifndef CLOUDLISTENER_H
#define CLOUDLISTENER_H

#include <QObject>
#include <QThread>
#include "cosclient.h"
#include "synctask.h"

class CloudListener: public QThread
{
    Q_OBJECT
public:
    QString computerName;
    QMap<QString,QString> otherDeviceMap;
    QMap<QString,QDateTime> lastSyncTimeMap;
    QStringList remotePaths;
    QVector<SyncTask>  myTasks;
    void setTasks(const QVector<SyncTask> &tasks);
    void setRemotePaths(const QStringList &paths);
    COSConfig config;
    COSClient *cosclient;
    CloudListener();
    CloudListener(COSConfig configi);
    void run() override;
signals:
    void cloudDirectoryChanged(SyncTask task,int type);
};

#endif // CLOUDLISTENER_H
