#ifndef SYNCTHREAD_H
#define SYNCTHREAD_H

#include "qthread.h"
#include "synctask.h"
#include "cosclient.h"
#include "xmlprocesser.h"
#include <QObject>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDateTime>
#include <QString>
#include "watcher.hpp"
using namespace wtr;
class SyncThread : public QThread
{
Q_OBJECT
public:
    int lastTaskId = 0;
    bool shouldListen = false;
    QString path;
    QString remotePath;
    int syncStatus;
    COSClient* cosclient;
    COSConfig config;
    SyncTask* task=nullptr;
    int getStatus();
    QString getRemotePath();
    qint64 totalSize=0;
    qint64 upFileSize=0;
    qint64 downFileSize=0;
    SyncThread(QString pathi, COSConfig configi, SyncTask *taski);
    SyncThread(QString pathi, COSConfig configi, QString remotePath, int syncStatus);
    void run() override;
public slots:
    void readDirectory(const QString &path);
    void recursiveRead(const QString &path);
    void readCloudDirectory(const QString &cloudpath);
    void addSynctask(const QFileInfo &info);//添加同步任务，把本地未上传文件添加入任务
    bool isTheSameFile(const QString &localPath, const QString &cloudPath);//判断本地文件和云端文件是否相同
    void onTaskCanceled(int fileTaskId);
signals:
    void localTotalSize(qint64);//本地文件总大小
    void upTotalSize(qint64);//上传文件总大小
    void newUploadTask(const QString &localPath, qint64 fileTaskId);
    void newDownloadTask(const QString &localPath, qint64 fileTaskId,quint64 totalSize);
    void updateUploadTask(int fileTaskId, qint64 nowSize, qint64 totalSize);
    void updateDownloadTask(int fileTaskId, qint64 nowSize, qint64 totalSize);
    void callUploadTask(const QString &localPath, const QString &cloudPath, int fileTaskId);
    void callDownloadTask(const QString &localPath, const QString &cloudPath, int fileTaskId);
    void callRenameFileTask(const QString &cloudPath, const QString &copyToPath, int fileTaskId);
    void callDeleteFileTask(const QString &cloudPath,const QString &versionId, int fileTaskId);
};






#endif // SYNCTHREAD_H
