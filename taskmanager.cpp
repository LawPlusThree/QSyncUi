#include "taskmanager.h"

//新建一个数据库用于存储任务信息并建表
void TaskManager::createConnection(QString account)
{
    db = QSqlDatabase::addDatabase("QSQLITE","taskManager");
    db.setDatabaseName(account+"_task.db");
    //如果数据库不存在则创建数据库
    if (!db.open())
    {
        qDebug() << "无法建立数据库连接";
        return;
    }
    QSqlQuery query(db);
    // 检查任务表是否存在
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='uptask'");
    if (!query.next()) {
        // 如果任务表不存在，则创建任务表
        query.exec("CREATE TABLE uptask (id INTEGER PRIMARY KEY, remotePath VARCHAR, localPath VARCHAR, dataSize BIGINT, totalPiece INTEGER, etags TEXT, isPause INTEGER)");
    }
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='downtask'");
    if (!query.next()) {
        query.exec("CREATE TABLE downtask (id INTEGER PRIMARY KEY, remotePath VARCHAR, localPath VARCHAR, dataSize BIGINT, totalPiece INTEGER, etags TEXT, isPause INTEGER)");
    }
    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='finishtask'");
    if (!query.next()) {
        query.exec("CREATE TABLE finishtask (id INTEGER PRIMARY KEY,taskId INTEGER, remotePath VARCHAR, localPath VARCHAR, dataSize BIGINT, sycnTime DATE, status INTEGER)");
    }

}

void TaskManager::insertUpTask(QString remotePath, QString localPath, quint64 dataSize, int totalPiece, QMap<int,QString> etags, bool isPause)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO uptask (remotePath, localPath, dataSize, totalPiece, etags, isPause) VALUES (:remotePath, :localPath, :dataSize, :totalPiece, :etags, :isPause)");
    query.bindValue(":remotePath", remotePath);
    query.bindValue(":localPath", localPath);
    query.bindValue(":dataSize", dataSize);
    query.bindValue(":totalPiece", totalPiece);
    query.bindValue(":etags", processEtag(etags));
    query.bindValue(":isPause", isPause);
    query.exec();
}

void TaskManager::insertDownTask(QString remotePath, QString localPath, quint64 dataSize, int totalPiece, QMap<int, QString> etags, bool isPause)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO downtask (remotePath, localPath, dataSize, totalPiece, etags, isPause) VALUES (:remotePath, :localPath, :dataSize, :totalPiece, :etags, :isPause)");
    query.bindValue(":remotePath", remotePath);
    query.bindValue(":localPath", localPath);
    query.bindValue(":dataSize", dataSize);
    query.bindValue(":totalPiece", totalPiece);
    query.bindValue(":etags", processEtag(etags));
    query.bindValue(":isPause", isPause);
    query.exec();
}

void TaskManager::insertFinishTask(int taskId,QString remotePath, QString localPath, quint64 dataSize, QDate sycnTime, int status)
{
    QSqlQuery query(db);
    query.prepare("INSERT INTO finishtask (taskId,remotePath, localPath, dataSize, sycnTime, status) VALUES (:taskId, :localPath, :dataSize, :sycnTime, :status)");
    query.bindValue(":taskId", taskId);
    query.bindValue(":remotePath", remotePath);
    query.bindValue(":localPath", localPath);
    query.bindValue(":dataSize", dataSize);
    query.bindValue(":sycnTime", sycnTime);
    query.bindValue(":status", status);
    query.exec();
}

void TaskManager::deleteUpTask(QString localPath)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM uptask WHERE localPath = :localPath");
    query.bindValue(":localPath", localPath);
    query.exec();
}

void TaskManager::deleteDownTask(QString localPath)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM downtask WHERE localPath = :localPath");
    query.bindValue(":localPath", localPath);
    query.exec();
}

void TaskManager::deleteFinishTask(QString localPath)
{
    QSqlQuery query(db);
    query.prepare("DELETE FROM finishtask WHERE localPath = :localPath");
    query.bindValue(":localPath", localPath);
    query.exec();
}

void TaskManager::updateUpTask(QString remotePath, QString localPath, quint64 dataSize, int totalPiece, QMap<int, QString> etags, bool isPause)
{
    QSqlQuery query(db);
    query.prepare("UPDATE uptask SET remotePath = :remotePath, dataSize = :dataSize, totalPiece = :totalPiece, etags = :etags, isPause = :isPause WHERE localPath = :localPath");
    query.bindValue(":remotePath", remotePath);
    query.bindValue(":localPath", localPath);
    query.bindValue(":dataSize", dataSize);
    query.bindValue(":totalPiece", totalPiece);
    query.bindValue(":etags", processEtag(etags));
    query.bindValue(":isPause", isPause);
    query.exec();
}

void TaskManager::updateDownTask(QString remotePath, QString localPath, quint64 dataSize, int totalPiece, QMap<int, QString> etags, bool isPause)
{
    QSqlQuery query(db);
    query.prepare("UPDATE downtask SET remotePath = :remotePath, dataSize = :dataSize, totalPiece = :totalPiece, etags = :etags, isPause = :isPause WHERE localPath = :localPath");
    query.bindValue(":remotePath", remotePath);
    query.bindValue(":localPath", localPath);
    query.bindValue(":dataSize", dataSize);
    query.bindValue(":totalPiece", totalPiece);
    query.bindValue(":etags", processEtag(etags));
    query.bindValue(":isPause", isPause);
    query.exec();
}

void TaskManager::updateFinishTask(int taskId,QString remotePath, QString localPath, quint64 dataSize, QDate sycnTime, int status)
{
    QSqlQuery query(db);
    query.prepare("UPDATE finishtask SET taskId = :taskId,remotePath = :remotePath, dataSize = :dataSize, sycnTime = :sycnTime, status = :status WHERE localPath = :localPath");
    query.bindValue(":taskId", taskId);
    query.bindValue(":remotePath", remotePath);
    query.bindValue(":localPath", localPath);
    query.bindValue(":dataSize", dataSize);
    query.bindValue(":sycnTime", sycnTime);
    query.bindValue(":status", status);
    query.exec();
}

QList<upTask> TaskManager::readUpTask()
{
    QList<upTask> tasks;
    QSqlQuery query(db);
    query.exec("SELECT * FROM uptask");
    while (query.next())
    {
        upTask task;
        task.remotePath = query.value(1).toString();
        task.localPath = query.value(2).toString();
        task.dataSize = query.value(3).toULongLong();
        task.totalPiece = query.value(4).toInt();
        task.etags = processJson(query.value(5).toString());
        task.isPause = query.value(6).toBool();
        tasks.append(task);
    }
    return tasks;
}

QList<downTask> TaskManager::readDownTask()
{
    QList<downTask> tasks;
    QSqlQuery query(db);
    query.exec("SELECT * FROM downtask");
    while (query.next())
    {
        downTask task;
        task.remotePath = query.value(1).toString();
        task.localPath = query.value(2).toString();
        task.dataSize = query.value(3).toULongLong();
        task.totalPiece = query.value(4).toInt();
        task.etags = processJson(query.value(5).toString());
        task.isPause = query.value(6).toBool();
        tasks.append(task);
    }
    return tasks;
}

QList<finishTask> TaskManager::readFinishTask()
{
    QList<finishTask> tasks;
    QSqlQuery query(db);
    query.exec("SELECT * FROM finishtask");
    while (query.next())
    {
        finishTask task;
        task.taskId = query.value(1).toInt();
        task.remotePath = query.value(2).toString();
        task.localPath = query.value(3).toString();
        task.dataSize = query.value(4).toULongLong();
        task.sycnTime = query.value(5).toDate();
        task.status = query.value(6).toInt();
        tasks.append(task);
    }
    return tasks;
}

void TaskManager::closeConnection()
{
    QSqlDatabase::database().close();
}

QMap<int, QString> TaskManager::processJson(QString etags)
{
    QMap<int, QString> etagMap;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(etags.toUtf8());
    QJsonArray jsonArray = jsonDocument.array();
    for(int i = 0; i < jsonArray.size(); i++)
    {
        QJsonObject jsonObject = jsonArray.at(i).toObject();
        etagMap.insert(jsonObject.keys().at(0).toInt(), jsonObject.value(jsonObject.keys().at(0)).toString());
    }
    return etagMap;
}

QString TaskManager::processEtag(QMap<int, QString> etags)
{
    // 把etag转化为json格式
    QJsonArray jsonArray;
    for(auto it = etags.begin(); it != etags.end(); it++)
    {
        QJsonObject jsonObject;
        jsonObject.insert(QString::number(it.key()), it.value());
        jsonArray.append(jsonObject);
    }
    QJsonDocument jsonDocument;
    jsonDocument.setArray(jsonArray);
    return jsonDocument.toJson();
}

