#include "filefunc.h"


void Filefunc::readDirectory(const QString &path)
{
    // 清空之前的文件列表
    fileInfoList.clear();

    // 递归读取文件夹和子文件夹
    recursiveRead(path);

    // 发出文件列表更新信号
    emit fileListUpdated(path, fileInfoList);
}

void Filefunc::recursiveRead(const QString &path)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QFileInfo &info, list) {
        if (info.isDir()) {
            // 如果是目录，递归读取
            recursiveRead(info.filePath());
        } else {
            // 如果是文件，添加到文件信息列表
            fileInfoList.append(info);
            qDebug()<<info.fileName();
        }
    }
}

bool Filefunc::comparetime(QDateTime localtime,QString cloudtimestr){
    //QString localtime=info.metadataChangeTime().toMSecsSinceEpoch();
    // 使用QString的fromString方法和Qt的ISODate格式来解析时间字符串
    QDateTime cloudtime=QDateTime::fromString(cloudtimestr, Qt::ISODate);
    return localtime>=cloudtime;
}


