#ifndef SYNCCORE_H
#define SYNCCORE_H
#include <QObject>
#include "watcher.hpp"
using namespace wtr;
class SyncTask;
class SyncCore : public QObject
{
    Q_OBJECT
public:
    explicit SyncCore(QObject *parent = nullptr);
    void addTask(SyncTask *task);
    void doTask(SyncTask *task);
    void filesystemChanged(struct event e);
private:
    QVector<QPair<SyncTask*,watch*>> watchers;
private slots:
    void onDirectoryChanged(const QString &path);
    void onFileChanged(const QString &path);
};

#endif // SYNCCORE_H
