#ifndef FILEFUNC_H
#define FILEFUNC_H

#include "qthread.h"
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
    Filefunc(QString path):path(path){};
    void run() override
    {
        readDirectory(path);
    }
    void readDirectory(const QString &path);
    void recursiveRead(const QString &path);
    QFileInfoList fileInfoList;
};






#endif // FILEFUNC_H
