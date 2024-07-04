#include "synctask.h"



void SyncTaskManager::addTask(const SyncTask &task) {
    QSqlQuery query;
    query.prepare("INSERT INTO SyncTasks (localPath, remotePath, syncStatus) VALUES (:localPath, :remotePath, :syncStatus)");
    query.bindValue(":localPath", task.localPath.absolutePath());
    query.bindValue(":remotePath", task.remotePath);
    query.bindValue(":syncStatus", task.syncStatus);
}

bool SyncTaskManager::deleteTask(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM SyncTasks WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool SyncTaskManager::updateTask(const SyncTask &task) {
    QSqlQuery query;
    // Update the task with the same id
    //CREATE TABLE IF NOT EXISTS SyncTasks (id INTEGER PRIMARY KEY AUTOINCREMENT, localPath TEXT, remotePath TEXT, syncStatus INTEGER)
    query.prepare("UPDATE SyncTasks SET localPath = :localPath, remotePath = :remotePath, syncStatus = :syncStatus WHERE id = :id");
    query.bindValue(":localPath", task.localPath.absolutePath());
    query.bindValue(":remotePath", task.remotePath);
    query.bindValue(":syncStatus", task.syncStatus);
    query.bindValue(":id", task.id);
    return query.exec();
}

bool SyncTaskManager::deleteATask(const SyncTask &task) {
    QSqlQuery query;
    // Update the task with the same id
    //CREATE TABLE IF NOT EXISTS SyncTasks (id INTEGER PRIMARY KEY AUTOINCREMENT, localPath TEXT, remotePath TEXT, syncStatus INTEGER)
    query.prepare("DELETE FROM SyncTasks WHERE id = :id");
    query.bindValue(":id", task.id);
    return query.exec();
}

bool SyncTaskManager::queryTask(const SyncTask &task) {
    QSqlQuery query;
    query.prepare("SELECT * FROM SyncTasks WHERE id = :id");
    query.bindValue(":id", task.id);
    return query.exec();
}

QList<SyncTask> SyncTaskManager::getTasks() {
    QList<SyncTask> tasks;
    QSqlQuery query("SELECT * FROM SyncTasks");
    while (query.next()) {
        int id = query.value("id").toInt();
        QString localPath = query.value("localPath").toString();
        QString remotePath = query.value("remotePath").toString();
        int syncStatus = query.value("syncStatus").toInt();
        SyncTask task(id,localPath, remotePath, syncStatus);
        tasks.append(task);
    }
    return tasks;
}

void SyncTaskManager::initializeDatabase(QString name) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //存到userData文件夹
    db.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/"+name+"syncTasks.db");
    if (!db.open()) {
        qDebug() << "Database open failed:" << db.lastError();
        return;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE IF NOT EXISTS SyncTasks (id INTEGER PRIMARY KEY AUTOINCREMENT, localPath TEXT, remotePath TEXT, syncStatus INTEGER)")) {
        qDebug() << "Failed to create table:" << query.lastError();
    }
}

SyncTask::SyncTask(int id,QString localPath, QString remotePath, int syncStatus){
    this->id=id;
    QDir dir(localPath);
    this->localPath=dir;
    this->remotePath=remotePath;
    this->syncStatus=syncStatus;
}
