#include "storage.h"

Storage::Storage(const QByteArray &c) {
    QNetworkAccessManager *cli = new QNetworkAccessManager();
    // 这里需要根据实际的配置设置请求头和其他参数
    this->minio = cli;
    /*this->bucket = c.remoteBucket;
    this->localPrefix = c.localPath;
    this->remotePrefix = c.remotePath;
    this->symLink = c.symLink;*/
}
