#ifndef SYNCCORE_H
#define SYNCCORE_H
#include <QObject>
#include "watcher.hpp"
#include <iostream>
using namespace wtr;
class SyncTask;
class SyncCore : public QObject
{
    Q_OBJECT
public:
    explicit SyncCore(QObject *parent = nullptr);
    void addTask(SyncTask *task);
    void generalCallback(struct event e);

private:
    QVector<watch*> watchers;
private slots:
    void onDirectoryChanged(const QString &path);
    void onFileChanged(const QString &path);
};

#endif // SYNCCORE_H
