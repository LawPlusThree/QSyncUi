#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include<Qtsql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlError>
#include <QDebug>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDate>

//数据库名字是"账户名_task.db"
//上传任务表名是uptask:
//字段有text remotePath,text localPath,bigint dataSize,int totalPiece,text etags, int isPause
//下载任务表名是downtask:
//字段有text remotePath,text localPath,bigint dataSize,int totalPiece,text etags, int isPause
//历史任务表名是finishtask:
//字段有text remotePath,text localPath,bigint dataSize,date sycnTime,int status

struct upTask
{
    int uploadId;
    QString remotePath;
    QString localPath;
    quint64 dataSize;
    int totalPiece;
    QMap<int,QString> etags;
    bool isPause;
    upTask(){};//默认构造函数
    //复制构造函数
    upTask(const upTask &task)
    {
        uploadId=task.uploadId;
        remotePath=task.remotePath;
        localPath=task.localPath;
        dataSize=task.dataSize;
        totalPiece=task.totalPiece;
        etags=task.etags;
        isPause=task.isPause;
    }
};

struct downTask
{
    QString remotePath;
    QString localPath;
    quint64 dataSize;
    int totalPiece;
    QMap<int,QString> etags;
    bool isPause;
    downTask(){};//默认构造函数
    //复制构造函数
    downTask(const downTask &task)
    {
        remotePath=task.remotePath;
        localPath=task.localPath;
        dataSize=task.dataSize;
        totalPiece=task.totalPiece;
        etags=task.etags;
        isPause=task.isPause;
    }
};

struct finishTask
{
    int taskId;
    QString remotePath;
    QString localPath;
    quint64 dataSize;
    QDate sycnTime;
    int status;//1表示上传，0表示下载
    finishTask(){};//默认构造函数
    //复制构造函数
    finishTask(const finishTask &task)
    {
        taskId=task.taskId;
        remotePath=task.remotePath;
        localPath=task.localPath;
        dataSize=task.dataSize;
        sycnTime=task.sycnTime;
        status=task.status;
    }
};

class TaskManager: public QObject
{
    Q_OBJECT
public:
    TaskManager(QString account){createConnection(account);};
    void createConnection(QString account);
    void insertUpTask(int uploadId,QString remotePath, QString localPath, quint64 dataSize,int totalPiece,QMap<int,QString> etags, bool isPause);
    void insertDownTask(QString remotePath, QString localPath, quint64 dataSize,int totalPiece,QMap<int,QString> etags, bool isPause);
    void insertFinishTask(int taskId,QString remotePath, QString localPath, quint64 dataSize, QDate sycnTime, int status);
    void deleteUpTask(QString localPath);
    void deleteDownTask(QString localPath);
    void deleteFinishTask(QString localPath);
    void updateUpTask(int uploadId,QString remotePath, QString localPath, quint64 dataSize,int totalPiece,QMap<int,QString> etags, bool isPause);
    void updateDownTask(QString remotePath, QString localPath, quint64 dataSize,int totalPiece,QMap<int,QString> etags, bool isPause);
    void updateFinishTask(int taskId,QString remotePath, QString localPath, quint64 dataSize, QDate sycnTime, int status);
    QList<upTask> readUpTask();
    QList<downTask> readDownTask();
    QList<finishTask> readFinishTask();
    void closeConnection();
    ~TaskManager(){closeConnection();};
    QMap<int,QString> processJson(QString etags);//处理数据库中以json格式存储的etag
    QString processEtag(QMap<int,QString> etags);//把etag转化为json格式
private:
    QSqlDatabase db;
};

#endif // TASKMANAGER_H
