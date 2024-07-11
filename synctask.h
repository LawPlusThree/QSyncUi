#ifndef SYNCTASK_H
#define SYNCTASK_H

#include <QDateTime>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QString>
#include "qcontainerfwd.h"
#include "cosclient.h"
//声明友元类
class SyncCore;
class SyncTaskDatabaseManager;
class User;
class SyncTask
{
private:
    //A local path to a dir
    int id;
    QDir localPath;
    QString remotePath;
    int syncStatus; //1 for upload/download,2 for only upload,3 for only download. -1 for paused
    //lastSyncTime, use Qt::currentDateTime() to get the current time
    QDateTime lastSyncTime=QDateTime::fromString("2000-01-01 00:00:00","yyyy-MM-dd hh:mm:ss");

public:
    SyncTask(QString localPath, QString remotePath, int syncStatus, int id=-1);
    SyncTask(const SyncTask &task){
        localPath = task.localPath;
        remotePath = task.remotePath;
        syncStatus = task.syncStatus;
        lastSyncTime = task.lastSyncTime;
        id = task.id;
    }
    COSClient* cosclient;
    QString getLocalPath() const { return localPath.absolutePath(); }
    QString getRemotePath() const { return remotePath; }
    int getId() const { return id; }
    int getSyncStatus() const { return syncStatus; }
    void setSyncStatus(int status) { syncStatus = status; }
    void setId(int id) { this->id = id; }
    QDateTime getLastSyncTime() const { return lastSyncTime; }
    void setLastSyncTime(QDateTime time) { lastSyncTime = time; }
    friend class SyncTaskDatabaseManager;
    friend class SyncCore;
};

class SyncTaskDatabaseManager
{
public:
    SyncTaskDatabaseManager(User* u);

    int addTask(const SyncTask &task);

    bool deleteTask(int id);

    bool updateTask(const SyncTask &task);

    bool deleteATask(const SyncTask &task);

    bool queryTask(const SyncTask &task);

    void updateTaskStatus(int id, int status);

    void updateTaskTime(int id, QDateTime time);

    void updateTaskRemotePath(int id, QString remotePath);

    void updateTaskLocalPath(int id, QString localPath);

    SyncTask getTaskById(int id);

    QList<SyncTask> getTasks();

    ~SyncTaskDatabaseManager() { db.close(); }

private:
    void initializeDatabase(QString name);
    QSqlDatabase db;
};
#endif // SYNCTASK_H
