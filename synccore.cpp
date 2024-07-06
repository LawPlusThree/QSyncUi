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
