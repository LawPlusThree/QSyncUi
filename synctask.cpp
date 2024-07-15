#include "synctask.h"
#include "user.h"


SyncTaskDatabaseManager::SyncTaskDatabaseManager(User *u) { initializeDatabase(u->getUserHash()); }

int SyncTaskDatabaseManager::addTask(const SyncTask &task) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO SyncTasks (localPath, remotePath, syncStatus, lastSyncTime) VALUES (:localPath, :remotePath, :syncStatus, :lastSyncTime)");
    query.bindValue(":localPath", task.localPath.absolutePath());
    query.bindValue(":remotePath", task.remotePath);
    query.bindValue(":syncStatus", task.syncStatus);
    query.bindValue(":lastSyncTime", task.lastSyncTime.toString("yyyy-MM-dd hh:mm:ss"));
    if (query.exec()){
        qDebug()<<"Insert success";
    }else{
        qDebug()<<"Insert failed"<<query.lastError();
    }
    //返回ID
    int result=query.lastInsertId().toInt();
    return result;
}

bool SyncTaskDatabaseManager::deleteTask(int id) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM SyncTasks WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool SyncTaskDatabaseManager::updateTask(const SyncTask &task) {
    QSqlQuery query(db);
    // Update the task with the same id
    //CREATE TABLE IF NOT EXISTS SyncTasks (id INTEGER PRIMARY KEY AUTOINCREMENT, localPath TEXT, remotePath TEXT, syncStatus INTEGER)
    query.prepare("UPDATE SyncTasks SET localPath = :localPath, remotePath = :remotePath, syncStatus = :syncStatus, lastSyncTime = :lastSyncTime WHERE id = :id");
    query.bindValue(":localPath", task.localPath.absolutePath());
    query.bindValue(":remotePath", task.remotePath);
    query.bindValue(":syncStatus", task.syncStatus);
    query.bindValue(":lastSyncTime", task.lastSyncTime.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":id", task.id);
    return query.exec();
}

bool SyncTaskDatabaseManager::deleteATask(const SyncTask &task) {
    QSqlQuery query(db);
    // Update the task with the same id
    //CREATE TABLE IF NOT EXISTS SyncTasks (id INTEGER PRIMARY KEY AUTOINCREMENT, localPath TEXT, remotePath TEXT, syncStatus INTEGER)
    query.prepare("DELETE FROM SyncTasks WHERE id = :id");
    query.bindValue(":id", task.id);
    return query.exec();
}

bool SyncTaskDatabaseManager::queryTask(const SyncTask &task) {
    QSqlQuery query(db);
    query.prepare("SELECT * FROM SyncTasks WHERE id = :id");
    query.bindValue(":id", task.id);
    return query.exec();
}

void SyncTaskDatabaseManager::updateTaskStatus(int id, int status)
{
    QSqlQuery query(db);
    query.prepare("UPDATE SyncTasks SET syncStatus = :status WHERE id = :id");
    query.bindValue(":status", status);
    query.bindValue(":id", id);
    query.exec();
}


void SyncTaskDatabaseManager::updateTaskTime(int id, QDateTime time)
{
    QSqlQuery query(db);
    query.prepare("UPDATE SyncTasks SET lastSyncTime = :time WHERE id = :id");
    query.bindValue(":time", time.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":id", id);
    query.exec();
}

void SyncTaskDatabaseManager::updateTaskRemotePath(int id, QString remotePath)
{
    QSqlQuery query(db);
    query.prepare("UPDATE SyncTasks SET remotePath = :remotePath WHERE id = :id");
    query.bindValue(":remotePath", remotePath);
    query.bindValue(":id", id);
    query.exec();
}

void SyncTaskDatabaseManager::updateTaskLocalPath(int id, QString localPath)
{
    QSqlQuery query(db);
    query.prepare("UPDATE SyncTasks SET localPath = :localPath WHERE id = :id");
    query.bindValue(":localPath", localPath);
    query.bindValue(":id", id);
    query.exec();
}

SyncTask SyncTaskDatabaseManager::getTaskById(int id)
{
    QSqlQuery query(db);
    query.prepare("SELECT * FROM SyncTasks WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    query.next();
    QString localPath = query.value("localPath").toString();
    QString remotePath = query.value("remotePath").toString();
    int syncStatus = query.value("syncStatus").toInt();
    SyncTask task(localPath, remotePath, syncStatus, id);
    task.lastSyncTime = QDateTime::fromString(query.value("lastSyncTime").toString(), "yyyy-MM-dd hh:mm:ss");
    return task;
}



QList<SyncTask> SyncTaskDatabaseManager::getTasks() {
    QList<SyncTask> tasks;
    QSqlQuery query("SELECT * FROM SyncTasks",db);
    while (query.next()) {
        int id = query.value("id").toInt();
        QString localPath = query.value("localPath").toString();
        QString remotePath = query.value("remotePath").toString();
        int syncStatus = query.value("syncStatus").toInt();
        SyncTask task(localPath, remotePath, syncStatus, id);
        task.lastSyncTime = QDateTime::fromString(query.value("lastSyncTime").toString(), "yyyy-MM-dd hh:mm:ss");
        tasks.append(task);
    }
    return tasks;
}

void SyncTaskDatabaseManager::initializeDatabase(QString name) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    //存到userData文件夹
    db.setDatabaseName(name+"syncTasks.db");
    if (!db.open()) {
        qDebug() << "Database open failed:" << db.lastError();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS SyncTasks (id INTEGER PRIMARY KEY AUTOINCREMENT, localPath TEXT, remotePath TEXT, syncStatus INTEGER, lastSyncTime TEXT)")) {
        qDebug() << "Failed to create table:" << query.lastError();
    }
}

SyncTask::SyncTask(QString localPath, QString remotePath, int syncStatus, int id /* = -1 */) {
    this->id = id; // 如果id是-1，表示这是一个新的任务，id将由数据库自动生成
    QDir dir(localPath);
    this->localPath = dir;
    this->remotePath = remotePath;
    this->syncStatus = syncStatus;
}


