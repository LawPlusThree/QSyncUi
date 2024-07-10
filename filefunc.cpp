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
        }
    }
}

void Filefunc::readCLoudDirectory(const QString &cloudpath)
{
    // 递归读取云文件夹和子文件夹
    recursiveReadCloud(cloudpath);
}

void Filefunc::recursiveReadCloud(const QString &cloudpath)
{
    // 读取云文件夹
    QString prefix = cloudpath;
    if (!prefix.endsWith("/")) {
        prefix += "/";
    }

    // 读取云文件夹下的文件和子文件夹
    preRequest request;
    request.queryParams["prefix"] = prefix;
    request.queryParams["delimiter"] = "/";
    preResponse response = cosclient->listObjects();
    QDomDocument doc;
    doc.setContent(response.data);
    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();
    while (!node.isNull()) {
        QDomElement element = node.toElement();
        if (element.tagName() == "Contents") {
            // 如果是文件，添加到文件信息列表
            QString key = element.firstChildElement("Key").text();
            QString cloudPath = key.mid(prefix.length());
            QString localPath = task->getLocalPath() + "/" + cloudPath;
            addSynctask(localPath);
        } else if (element.tagName() == "CommonPrefixes") {
            // 如果是子文件夹，递归读取
            QString subPrefix = element.firstChildElement("Prefix").text();
            recursiveReadCloud(subPrefix);
        }
        node = node.nextSibling();
    }

}

void Filefunc::addSynctask(const QString &path)
{
    //把path前面和task->getLocalPath()相同的部分去掉
    QString relativePath=path.mid(task->getLocalPath().length()+1);
    QString cloudPath=task->getRemotePath()+relativePath;
    headHeader tmpHeaders;
    preResponse response=cosclient->headObject(cloudPath,"",tmpHeaders);
    if(!cosclient->isExist(response)){
        qDebug()<<"正在同步:"<<path;
        cosclient->putLocalObject(cloudPath,path);
    }
}


