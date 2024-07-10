#ifndef FILEFUNC_H
#define FILEFUNC_H

#include "qthread.h"
#include "synctask.h"
#include "cosclient.h"
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
    Filefunc(QString pathi,COSClient* cosclienti,SyncTask* taski):path(pathi),cosclient(cosclienti),task(taski){};
    void run() override
    {
        readDirectory(path);
    }
    void readDirectory(const QString &path);
    void recursiveRead(const QString &path);

    void addSynctask(const QString &path);//添加同步任务，把本地未上传文件添加入任务
    void deleteSynctask(const QString &path);//删除同步任务，把本地已上传文件删除任务
    void updateSynctask(const QString &path);//更新同步任务，把本地已上传文件更新任务
};






#endif // FILEFUNC_H
