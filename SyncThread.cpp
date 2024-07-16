#include "SyncThread.h"
#include "crc64util.h"
#include "globalvalue.h"
// 用多线程遍历本地文件夹
void SyncThread::run()
{
    crc64_init();
    if (task->getSyncStatus() == 1)
    {
        readDirectory(path);
        readCloudDirectory(task->getRemotePath());
    }
    else if (task->getSyncStatus() == 2)
    {
        readDirectory(path);
    }
    else if (task->getSyncStatus() == 3)
    {
        readCloudDirectory(task->getRemotePath());
    }
    emit this->localTotalSize(totalSize);
    emit this->upTotalSize(upFileSize);
    QDir listen = task->getLocalPath();

    if (task->getSyncStatus() == 1 || task->getSyncStatus() == 2)
    {
        qDebug() << "upload/download";
        auto path=listen.filesystemAbsolutePath();
        watch *w = new watch(path, [this](struct event e)
                             {
    QFileInfo info(e.path_name);
    QString path = info.absoluteFilePath();
    QString relativePath = path.mid(task->getLocalPath().length() + 1);
    QString cloudPath = task->getRemotePath() + relativePath;
    // 读取 path_type 状态
    // 读取 effect_type 状态
    switch (e.effect_type) {
    case event::effect_type::rename:
        qDebug() << "Effect type: Rename";
        break;
    case event::effect_type::modify:{
        qDebug() << "Effect type: Modify";
        if (isTheSameFile(path, cloudPath))
        {
            qDebug() << "Same file";
        }
        else
        {
            qDebug() << "Different file";
            QFile file(path);
            //是否可读
            if (!file.open(QIODevice::ReadOnly))
            {
                break;
            }
            int fileTaskId = getNextFileTaskId();
            emit this->newUploadTask(path, fileTaskId);
            emit this->callUploadTask(path, cloudPath, fileTaskId);
        }
        break;
    }
    case event::effect_type::create:{
        qDebug() << "Effect type: Create";
        QFile file(path);
        //是否可写
        if (!file.open(QIODevice::ReadWrite))
        {
            break;
        }
        int fileTaskId = getNextFileTaskId();
        emit this->newUploadTask(path, fileTaskId);
        emit this->callUploadTask(path, cloudPath, fileTaskId);
        break;
    }
    case event::effect_type::destroy:{
        qDebug() << "Effect type: Destroy";
        int fileTaskId = getNextFileTaskId();
        emit this->callDeleteFileTask(cloudPath, "", fileTaskId);
        break;
    }
    case event::effect_type::owner:
        qDebug() << "Effect type: Owner change";
        break;
    case event::effect_type::other:
        qDebug() << "Effect type: Other";
        break;
    } });
    }
}

void SyncThread::readDirectory(const QString &path)
{
    // 递归读取文件夹和子文件夹
    recursiveRead(path);
}

void SyncThread::recursiveRead(const QString &path)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QFileInfo &info, list)
    {
        if (info.isDir())
        {
            // 如果是目录，递归读取
            recursiveRead(info.filePath());
        }
        else
        {
            // 如果是文件，添加到文件信息列表
            qDebug() << info.fileName();
            totalSize += info.size();
            addSynctask(info);
        }
    }
}

void SyncThread::readCloudDirectory(const QString &cloudpath)
{
    XmlProcesser processer;
    QString xml;
    Bucket bucket;
    bucket.isTruncated = false;
    bucket.nextMarker = "";
    do
    {
        xml = cosclient->listObjects(cloudpath, bucket.nextMarker);
        bucket = processer.processXml(xml);
        for (auto ct : bucket.contents)
        {
            // 不保留key的md5
            QString localPath = task->getLocalPath() + "/" + ct.key.mid(task->getRemotePath().length() + 33);
            QFileInfo info(localPath);
            bool needDownload = false;
            if (!info.exists())
            {
                needDownload = true;
            }
            else if (info.isDir())
            {
                // 如果是文件夹，直接跳过
                continue;
            }
            else
            {
                headHeader tmpHeaders;
                preResponse response = cosclient->headObject(ct.key, "", tmpHeaders);
                // read local file
                QFile file(localPath);
                file.open(QIODevice::ReadOnly);
                QByteArray data = file.readAll();
                file.close();
                uint64_t crc64_data = 0;
                crc64_data = crc64(crc64_data, data.data(), data.size());
                uint64_t cloudCRC = response.headers["x-cos-hash-crc64ecma"].toULongLong();
                if (crc64_data != cloudCRC)
                {
                    needDownload = true;
                }
            }
            if (needDownload)
            {
                downFileSize += ct.size;
                int fileTaskId = getNextFileTaskId();
                emit this->newDownloadTask(localPath, fileTaskId, ct.size);
                emit this->callDownloadTask(localPath, ct.key, fileTaskId);
            }
        }
    } while (bucket.isTruncated);
}

void SyncThread::fileSystemChanged(struct event e)
{
    qDebug() << e.path_name;
}

void SyncThread::addSynctask(const QFileInfo &info)
{
    // 把path前面和task->getLocalPath()相同的部分去掉
    QString path = info.absoluteFilePath();
    QString relativePath = path.mid(task->getLocalPath().length() + 1);
    QString cloudPath = task->getRemotePath() + relativePath;
    headHeader tmpHeaders;
    preResponse response = cosclient->headObject(cloudPath, "", tmpHeaders);
    if (cosclient->isExist(response)){

    }
    else
    {
        upFileSize += info.size();
        int fileTaskId = getNextFileTaskId();
        emit this->newUploadTask(path, fileTaskId);
        emit this->callUploadTask(path, cloudPath, fileTaskId);
    }
}


bool SyncThread::isTheSameFile(const QString &localPath, const QString &cloudPath="")
{
    QString _cloudPath = cloudPath;
    if(_cloudPath.isEmpty())
    {
        _cloudPath = task->getRemotePath() + localPath.mid(task->getLocalPath().length() + 1);
    }
    bool result = false;
    QFile file(localPath);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    uint64_t crc64_data = 0;
    crc64_data = crc64(crc64_data, data.data(), data.size());
    headHeader tmpHeaders;
    preResponse response = cosclient->headObject(_cloudPath, "", tmpHeaders);
    if(!cosclient->isExist(response)){
        return false;
    }
    uint64_t cloudCRC = response.headers["x-cos-hash-crc64ecma"].toULongLong();
    if (crc64_data == cloudCRC)
    {
        result = true;
    }
    return result;
}

void SyncThread::onTaskCanceled(int fileTaskId)
{
}
