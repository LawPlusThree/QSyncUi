#include "filefunc.h"


//用多线程遍历本地文件夹
void Filefunc::readDirectory(const QString &path)
{
    // 递归读取文件夹和子文件夹
    recursiveRead(path);
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
            addSynctask(info.absoluteFilePath());
            qDebug()<<info.fileName();
        }
    }
}

void Filefunc::addSynctask(const QString &path)
{

}


