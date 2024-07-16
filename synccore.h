#ifndef SYNCCORE_H
#define SYNCCORE_H
#include <QObject>
#include "qfileinfo.h"
#include "synctask.h"
#include "watcher.hpp"
#include "cosclient.h"
#include "filequeue.h"
using namespace wtr;
class SyncTask;
class SyncCore : public QObject
{
    Q_OBJECT
public:
    explicit SyncCore(QObject *parent = nullptr);
    SyncCore(COSConfig config, QObject *parent = nullptr, QString excludedItems = "");
    bool addTask(SyncTask *task);
    void doTask(SyncTask *task);
    NetworkRequestManager *requestManager;
signals:
    void taskTotalSize(qint64 size,int taskid);
    void taskUploadSize(qint64 size,int taskid);
    void addFileUploadTask(const QString &localPath, int fileTaskId);
    void addFileDownloadTask(const QString &localPath , int fileTaskId,quint64 totalSize=0);
    void updateFileUploadTask(int fileTaskId, qint64 nowSize,qint64 totalSize);
    void updateFileDownloadTask(int fileTaskId, qint64 nowSize,qint64 totalSize);
    void finishFileUploadTask(int fileTaskId);
    void finishFileDownloadTask(int fileTaskId);
    void cloudDirectoryChanged(const QString &cloudPath);
public slots:
    void pauseTask(int taskid){};
    void pauseAllTask(){};
    void pauseFileTask(int fileTaskId){};
private:
    COSConfig config;
    QVector<watch*> watchers;
    QVector<SyncTask *> tasks;
    QString excludedItems;
    SyncTask* findTaskByLocalPath(const QString &path){
        for (auto task : tasks) {
            if (task->localPath.absolutePath() == path){
                return task;
            }
        }
        return nullptr;
    }
public slots:
    void onDirectoryChanged(const QString &path);
    void onFileChanged(const QString &path);
    void onCloudDirectoryChanged(const QString &cloudPath){
        emit cloudDirectoryChanged(cloudPath);
    }
};

#endif // SYNCCORE_H
