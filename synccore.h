#ifndef SYNCCORE_H
#define SYNCCORE_H
#include <QObject>
#include "qfileinfo.h"
#include "synctask.h"
#include "watcher.hpp"
#include <iostream>
using namespace wtr;
class SyncTask;
class SyncCore : public QObject
{
    Q_OBJECT
public:
    explicit SyncCore(QObject *parent = nullptr);
    void addTask(SyncTask *task);
    void doTask(SyncTask *task);
    void filesystemChanged(struct event e);
signals:
    void addToUploadQueue(const QString &path, const QString &remotePath);
    void addToDownloadQueue(const QString &path, const QString &remotePath);
private:
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
    void onFileListUpdated(const QString &path, const QFileInfoList &list);
};

#endif // SYNCCORE_H
