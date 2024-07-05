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
#include "user.h"

//声明友元类
class SyncTaskManager;
class SyncTask
{
private:
    //A local path to a dir
    int id;
    QDir localPath;
    QString remotePath;
    int syncStatus; //1 for upload/download,2 for only upload,3 for only download. -1 for paused
    //lastSyncTime, use Qt::currentDateTime() to get the current time
    QDateTime lastSyncTime;

public:
    SyncTask(int id, QString localPath, QString remotePath, int syncStatus);
    friend class SyncTaskManager;
};

class SyncTaskManager
{
public:
    SyncTaskManager(User u) { initializeDatabase(u.getUserHash()); }

    void addTask(const SyncTask &task);

    bool deleteTask(int id);

    bool updateTask(const SyncTask &task);

    bool deleteATask(const SyncTask &task);

    bool queryTask(const SyncTask &task);

    QList<SyncTask> getTasks();

    // 其他方法：修改、删除、查询等

private:
    void initializeDatabase(QString name);
};
#endif // SYNCTASK_H
