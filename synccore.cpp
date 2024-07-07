#include "synccore.h"
#include "synctask.h"
using namespace wtr;
SyncCore::SyncCore(QObject *parent)
    : QObject{parent}
{
    //connect(&watcher, &QFileSystemWatcher::directoryChanged, this, &SyncCore::onDirectoryChanged);
    //connect(&watcher, &QFileSystemWatcher::fileChanged, this, &SyncCore::onFileChanged);
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
void SyncCore::addTask(SyncTask *task)
{
    QDir listen=task->localPath;
    if(task==nullptr)
        return;
    if(task->localPath.exists()==false)
    {
        qDebug()<<"local path not exists";
        return;
    }
    if(task->syncStatus==-1)
    {
        qDebug()<<"paused";
        return;
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
        return;
    }
    //watcher.addPath(listen.absolutePath());
}

void SyncCore::doTask(SyncTask *task)
{

}



void SyncCore::onDirectoryChanged(const QString &path)
{
    qDebug() << "Directory changed: " << path;
}

void SyncCore::onFileChanged(const QString &path)
{
    qDebug() << "File changed: " << path;
}
