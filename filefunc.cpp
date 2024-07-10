#include "filefunc.h"

//用多线程遍历本地文件夹
void Filefunc::readDirectory(const QString &path)
{
    // 递归读取文件夹和子文件夹
    recursiveRead(path);
    //emit localTotalSize(totalSize);
    //emit upTotalSize(upFileSize);
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
            totalSize+=info.size();
            addSynctask(info);
        }
    }
}

void Filefunc::readCLoudDirectory(const QString &cloudpath)
{
    XmlProcesser processer;
    QString xml;
    Bucket bucket;
    bucket.isTruncated=false;
    bucket.nextMarker="";
    do{
        xml=cosclient->listObjects(cloudpath,bucket.nextMarker);
        bucket=processer.processXml(xml);
    }while(!bucket.isTruncated);

}

void Filefunc::addSynctask(const QFileInfo &info)
{
    //把path前面和task->getLocalPath()相同的部分去掉
    QString path=info.absoluteFilePath();
    QString relativePath=path.mid(task->getLocalPath().length()+1);
    QString cloudPath=task->getRemotePath()+relativePath;
    headHeader tmpHeaders;
    preResponse response=cosclient->headObject(cloudPath,"",tmpHeaders);
    if(!cosclient->isExist(response)){
        upFileSize+=info.size();
        cosclient->putLocalObject(cloudPath,path);
    }
}


