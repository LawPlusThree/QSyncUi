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
    SyncCore(COSConfig config, QObject *parent = nullptr);
    bool addTask(SyncTask *task);
    void doTask(SyncTask *task);
    void filesystemChanged(struct event e);
    NetworkRequestManager *requestManager;
signals:
    void taskTotalSize(qint64 size,int taskid);
    void taskUploadSize(qint64 size,int taskid);
    void addFileUploadTask(const QString &localPath, int fileTaskId);
    void addFileDownloadTask(const QString &localPath , int fileTaskId);
    void updateFileUploadTask(int fileTaskId, qint64 nowSize,qint64 totalSize);
    void updateFileDownloadTask(int fileTaskId, qint64 nowSize,qint64 totalSize);
    void finishFileUploadTask(int fileTaskId);
    void finishFileDownloadTask(int fileTaskId);
public slots:
    void pauseTask(int taskid){};
    void pauseAllTask(){};
    void pauseFileTask(int fileTaskId){};
private:
    COSConfig config;
    QVector<watch*> watchers;
    QVector<SyncTask *> tasks;
    SyncTask* findTaskByLocalPath(const QString &path){
        for (auto task : tasks) {
            if (task->localPath.absolutePath() == path){
                return task;
            }
        }
        return nullptr;
    }
private slots:
    void onDirectoryChanged(const QString &path);
    void onFileChanged(const QString &path);
};

#endif // SYNCCORE_H
