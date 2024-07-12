#include "synccore.h"
#include "SyncThread.h"
#include <iostream>
using namespace wtr;
SyncCore::SyncCore(QObject *parent)
    : QObject{parent}
{
    qDebug()<<"sync core";
    //connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &SyncCore::onDirectoryChanged);
    //connect(&watcher, &QFileSystemWatcher::fileChanged, this, &SyncCore::onFileChanged);
}

SyncCore::SyncCore(COSConfig config, QObject *parent)
    : QObject{parent}, config{config}
{
    //connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &SyncCore::onDirectoryChanged);
    //connect(&watcher, &QFileSystemWatcher::fileChanged, this, &SyncCore::onFileChanged);
    requestManager=new NetworkRequestManager(config);
    connect(requestManager,&NetworkRequestManager::requestFinished,this,[=](int fileTaskId, QNetworkReply::NetworkError error){
        if(error==QNetworkReply::NoError)
        {
            emit finishFileDownloadTask(fileTaskId);
            qDebug()<<"request finished";
        }
        else
        {
            qDebug()<<"request error";
        }
    },Qt::BlockingQueuedConnection);
    connect(requestManager,&NetworkRequestManager::requestProgress,this,[=](int fileTaskId, qint64 bytesReceived, qint64 bytesTotal){
        //emit updateFileUploadTask(fileTaskId,bytesReceived,bytesTotal);
        qDebug()<<"request progress"<<bytesReceived<<" "<<bytesTotal;
        emit updateFileDownloadTask(fileTaskId,bytesReceived,bytesTotal);
    },Qt::BlockingQueuedConnection);
}

void SyncCore::filesystemChanged(struct event e)
{
    //e.associated;
    qDebug()<<e.path_name;
    // 读取 path_type 状态
    switch (e.path_type) {
    case event::path_type::dir:
        std::cout << "Path type: Directory" << std::endl;
        break;
    case event::path_type::file:
        std::cout << "Path type: File" << std::endl;
        break;
    case event::path_type::hard_link:
        std::cout << "Path type: Hard Link" << std::endl;
        break;
    case event::path_type::sym_link:
        std::cout << "Path type: Symbolic Link" << std::endl;
        break;
    case event::path_type::watcher:
        std::cout << "Path type: Watcher" << std::endl;
        break;
    case event::path_type::other:
        std::cout << "Path type: Other" << std::endl;
        break;
    }

    // 读取 effect_type 状态
    switch (e.effect_type) {
    case event::effect_type::rename:
        std::cout << "Effect type: Rename" << std::endl;
        break;
    case event::effect_type::modify:
        std::cout << "Effect type: Modify" << std::endl;
        break;
    case event::effect_type::create:
        std::cout << "Effect type: Create" << std::endl;
        break;
    case event::effect_type::destroy:
        std::cout << "Effect type: Destroy" << std::endl;
        break;
    case event::effect_type::owner:
        std::cout << "Effect type: Owner change" << std::endl;
        break;
    case event::effect_type::other:
        std::cout << "Effect type: Other" << std::endl;
        break;
    }
}
bool SyncCore::addTask(SyncTask *task)
{
    QDir listen=task->localPath;
    //Filefunc *filefunc=new Filefunc(this);
    //connect(filefunc, &Filefunc::fileListUpdated, this, &SyncCore::onFileListUpdated, Qt::QueuedConnection);
    //filefunc->readDirectory(listen.absolutePath());
    if(task==nullptr)
        return false;
    if(task->localPath.exists()==false)
    {
        qDebug()<<"local path not exists";
        return false;
    }
    if(task->syncStatus==-1)
    {
        qDebug()<<"paused";
        return false;
    }
    else if(task->syncStatus==1)
    {
        qDebug()<<"upload/download";
        watch* w=new watch(listen.absolutePath().toStdString(),[this](struct event e){this->filesystemChanged(e);});
        watchers.push_back(w);
    }
    else if(task->syncStatus==2)
    {
        qDebug()<<"only upload";
        watch* w=new watch(listen.absolutePath().toStdString(),[this](struct event e){this->filesystemChanged(e);});
        watchers.push_back(w);
    }
    else
    {
        qDebug()<<"only download";
    }
    tasks.push_back(task);
    doTask(task);
    return true;
}

void SyncCore::doTask(SyncTask *task)
{

    SyncThread *thread=new SyncThread(task->localPath.absolutePath(),config,task);
    connect(thread,&SyncThread::localTotalSize,this,[=](qint64 size){
        emit taskTotalSize(size,task->getId());
    });
    connect(thread,&SyncThread::upTotalSize,this,[=](qint64 size){
        emit taskUploadSize(size,task->getId());
    });
    connect(thread,&SyncThread::newUploadTask,this,[=](const QString &localPath, qint64 fileTaskId){
        emit addFileUploadTask(localPath,fileTaskId);
    });
    connect(thread,&SyncThread::newDownloadTask,this,[=](const QString &localPath, qint64 fileTaskId){
        emit addFileDownloadTask(localPath,fileTaskId);
        },Qt::QueuedConnection);
    connect(thread,&SyncThread::updateUploadTask,this,[=](int fileTaskId, qint64 nowSize, qint64 totalSize){
        emit updateFileUploadTask(fileTaskId,nowSize,totalSize);
    },Qt::QueuedConnection);
    connect(thread,&SyncThread::updateDownloadTask,this,[=](int fileTaskId, qint64 nowSize, qint64 totalSize){
        emit updateFileDownloadTask(fileTaskId,nowSize,totalSize);
    },Qt::QueuedConnection);
    connect(thread,&SyncThread::finishUploadTask,this,[=](int fileTaskId){
        emit finishFileUploadTask(fileTaskId);
    },Qt::QueuedConnection);
    connect(thread,&SyncThread::finishDownloadTask,this,[=](int fileTaskId){
        emit finishFileDownloadTask(fileTaskId);
    },Qt::QueuedConnection);
    connect(thread,&SyncThread::callUploadTask,this,[=](const QString &localPath, const QString &cloudPath, int fileTaskId){
        requestManager->addPutObjectRequest(localPath,cloudPath,fileTaskId,QMap<QString,QString>());
    },Qt::BlockingQueuedConnection);
    connect(thread,&SyncThread::callDownloadTask,this,[=](const QString &localPath, const QString &cloudPath, int fileTaskId){
        requestManager->addSave2LocalRequest(cloudPath,localPath,fileTaskId);
    },Qt::BlockingQueuedConnection);
    thread->start();
}



void SyncCore::onDirectoryChanged(const QString &path)
{
    qDebug() << "Directory changed: " << path;
}

void SyncCore::onFileChanged(const QString &path)
{
    qDebug() << "File changed: " << path;
}


