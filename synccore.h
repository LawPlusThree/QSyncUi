#ifndef SYNCCORE_H
#define SYNCCORE_H
#include <QObject>
#include <QFileSystemWatcher>
class SyncTask;
class SyncCore : public QObject
{
    Q_OBJECT
public:
    explicit SyncCore(QObject *parent = nullptr);
    void addTask(SyncTask *task);
private:
     QFileSystemWatcher watcher;
private slots:
    void onDirectoryChanged(const QString &path);
    void onFileChanged(const QString &path);
};

#endif // SYNCCORE_H
