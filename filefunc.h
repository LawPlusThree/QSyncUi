#ifndef FILEFUNC_H
#define FILEFUNC_H

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

class Filefunc : public QThread
{

public:
    QString path;
    COSClient* cosclient;
    SyncTask* task;
    qint64 totalSize=0;
    qint64 upFileSize=0;
    qint64 downFileSize=0;
    Filefunc(QString pathi,COSClient* cosclienti,SyncTask* taski):path(pathi),cosclient(cosclienti),task(taski){};
    void run() override
    {
        readDirectory(path);
    }
    void readDirectory(const QString &path);
    void recursiveRead(const QString &path);
    void readCLoudDirectory(const QString &cloudpath);

    void addSynctask(const QFileInfo &info);//添加同步任务，把本地未上传文件添加入任务
    void deleteSynctask(const QString &path);//删除同步任务，把本地已上传文件删除任务
    void updateSynctask(const QString &path);//更新同步任务，把本地已上传文件更新任务
signals:
    void localTotalSize(qint64 totalSize);//本地文件总大小
    void upTotalSize(qint64 upFileSize);//上传文件总大小
};






#endif // FILEFUNC_H
