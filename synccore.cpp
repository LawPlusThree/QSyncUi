#include "synccore.h"
#include "synctask.h"
using namespace wtr;
SyncCore::SyncCore(QObject *parent)
    : QObject{parent}
{
    //connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &SyncCore::onDirectoryChanged);
    //connect(&watcher, &QFileSystemWatcher::fileChanged, this, &SyncCore::onFileChanged);
}
void SyncCore::generalCallback(struct event e)
{
    qDebug()<<e.path_name;
}
void SyncCore::addTask(SyncTask *task)
{
    QDir listen=task->localPath;
    watch* w=new watch(listen.absolutePath().toStdString(),[this](struct event e){this->generalCallback(e);});
    watchers.push_back(w);
    //watcher.addPath(listen.absolutePath());
}



void SyncCore::onDirectoryChanged(const QString &path)
{
    qDebug() << "Directory changed: " << path;
}

void SyncCore::onFileChanged(const QString &path)
{
    qDebug() << "File changed: " << path;
}
