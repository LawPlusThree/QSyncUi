#include "synccore.h"
#include "synctask.h"
SyncCore::SyncCore(QObject *parent)
    : QObject{parent}
{
    connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &SyncCore::onDirectoryChanged);
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &SyncCore::onFileChanged);
}

void SyncCore::addTask(SyncTask *task)
{
    QDir listen=task->localPath;
    watcher.addPath(listen.absolutePath());
}

void SyncCore::onDirectoryChanged(const QString &path)
{
    qDebug() << "Directory changed: " << path;
}

void SyncCore::onFileChanged(const QString &path)
{
    qDebug() << "File changed: " << path;
}
