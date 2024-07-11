#include "SyncThread.h"
#include "crc64util.h"
//用多线程遍历本地文件夹
int SyncThread::fileTaskId=0;
void SyncThread::run()
{
    crc64_init();
    //readDirectory(path);
    readCLoudDirectory(task->getRemotePath());
    emit this->localTotalSize(totalSize);
    emit this->upTotalSize(upFileSize);
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

    foreach (const QFileInfo &info, list) {
        if (info.isDir()) {
            // 如果是目录，递归读取
            recursiveRead(info.filePath());
        } else {
            // 如果是文件，添加到文件信息列表
            qDebug()<<info.fileName();
            totalSize+=info.size();
            addSynctask(info);
        }
    }
}

void SyncThread::readCLoudDirectory(const QString &cloudpath)
{
    XmlProcesser processer;
    QString xml;
    Bucket bucket;
    bucket.isTruncated=false;
    bucket.nextMarker="";
    do{
        xml=cosclient->listObjects(cloudpath,bucket.nextMarker);
        bucket=processer.processXml(xml);
        for(auto ct:bucket.contents){
            qDebug()<<"云文件:"<<ct.key;
            QString localPath=task->getLocalPath()+"/"+ct.key.mid(task->getRemotePath().length());
            QFileInfo info(localPath);
            if(!info.exists()){
                QMap<QString,QString> metaDatas;
                cosclient->save2Local(ct.key,localPath,"",metaDatas);
            }
            else if(info.isDir()){
                //如果是文件夹，直接跳过
                continue;
            }
            else{
                headHeader tmpHeaders;
                preResponse response=cosclient->headObject(ct.key,"",tmpHeaders);
                //read local file
                QFile file(localPath);
                file.open(QIODevice::ReadOnly);
                QByteArray data=file.readAll();
                file.close();
                uint64_t crc64_data=0;
                crc64_data=crc64(crc64_data,data.data(),data.size());
                uint64_t cloudCRC=response.headers["x-cos-hash-crc64ecma"].toULongLong();
                if(crc64_data!=cloudCRC){
                    qDebug()<<"文件不一致:"<<localPath<<" "<<ct.key;
                    qDebug()<<"本地crc64:"<<crc64_data;
                    qDebug()<<"云crc64:"<<cloudCRC;
                    //更新文件
                    QMap<QString,QString> metaDatas;
                    cosclient->save2Local(ct.key,localPath,"",metaDatas);
                }
            }


        }
    }while(bucket.isTruncated);

}

void SyncThread::addSynctask(const QFileInfo &info)
{
    //把path前面和task->getLocalPath()相同的部分去掉
    QString path=info.absoluteFilePath();
    QString relativePath=path.mid(task->getLocalPath().length()+1);
    QString cloudPath=task->getRemotePath()+relativePath;
    headHeader tmpHeaders;
    preResponse response=cosclient->headObject(cloudPath,"",tmpHeaders);
    if(!cosclient->isExist(response)){
        upFileSize+=info.size();
        connect(cosclient,&COSClient::UploadProgress,[=](qint64 nowSize,qint64 total){
            emit this->updateUploadTask(path,nowSize);
        });

        cosclient->putLocalObject(cloudPath,path);
    }
}


