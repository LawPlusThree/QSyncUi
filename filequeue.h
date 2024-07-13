#ifndef FILEQUEUE_H
#define FILEQUEUE_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "cosclient.h"

struct RequestInfo{
    QString key;
    QString localPath;
    QString versionId;
    int methodId;
};

class NetworkRequestManager : public QObject {
    Q_OBJECT

public:
    NetworkRequestManager(const COSConfig &config, QObject *parent = nullptr)
        : QObject(parent), cosConfig(config), maxConcurrentRequests(3) {}

    void addPutObjectRequest(const QString &localPath, const QString &key, int fileTaskId, QMap<QString, QString> metaData) {
        QMutexLocker locker(&mutex);
        RequestInfo requestInfo;
        requestInfo.key = key;
        requestInfo.localPath = localPath;
        requestInfo.methodId = 0;
        requestQueue.enqueue({requestInfo , fileTaskId});
        if (activeRequests < maxConcurrentRequests) {
            startNextRequest();
        }
    }

    void addSave2LocalRequest(const QString &key, const QString &localPath, int fileTaskId) {
        QMutexLocker locker(&mutex);
        RequestInfo requestInfo;
        requestInfo.key = key;
        requestInfo.localPath = localPath;
        requestInfo.methodId = 1;
        requestQueue.enqueue({requestInfo, fileTaskId});
        if (activeRequests < maxConcurrentRequests) {
            startNextRequest();
        }
    }

signals:
    void putObjectRequested(const QString &key, const QString &localPath, QMap<QString, QString> metaData);
    void save2LocalRequested(const QString &key, const QString &localPath);
    void requestProgress(int fileTaskId, qint64 bytesReceived, qint64 bytesTotal);
    void requestFinished(int fileTaskId, QNetworkReply::NetworkError error);
public slots:
    void setMaxConcurrentRequests(int max) {
        qDebug() << "setMaxConcurrentRequests: " << max;
        QMutexLocker locker(&mutex);
        maxConcurrentRequests.store(max);
    }
private slots:
    void onRequestFinished(int fileTaskId, QNetworkReply::NetworkError error) {
        emit requestFinished(fileTaskId, error);
        QMutexLocker locker(&mutex);
        activeRequests--;
        startNextRequest();
    }

    void onRequestProgress(int fileTaskId, qint64 bytesReceived, qint64 bytesTotal) {
        qDebug() << "progress: " << bytesReceived << " / " << bytesTotal;
        emit requestProgress(fileTaskId, bytesReceived, bytesTotal);
    }

private:
    void startNextRequest() {
        if (requestQueue.isEmpty()) {
            return;
        }

        auto requestPair = requestQueue.dequeue();
        auto requestInfo = requestPair.first;
        int fileTaskId = requestPair.second;

        QThread *thread = QThread::create([=] {
            COSClient cosClient(cosConfig);
            connect(&cosClient, &COSClient::progress, this, [=](qint64 bytesReceived, qint64 bytesTotal) {
                onRequestProgress(fileTaskId, bytesReceived, bytesTotal);
            },Qt::BlockingQueuedConnection);
            connect(&cosClient, &COSClient::finished, this, [=](QNetworkReply::NetworkError error){
                onRequestFinished(fileTaskId, error);
                },Qt::BlockingQueuedConnection);
            if (requestInfo.methodId == 0) {
                cosClient.blockingPutFinishSignal=true;
                cosClient.multiUpload(requestInfo.key, requestInfo.localPath, QMap<QString, QString>());
            } else if (requestInfo.methodId == 1) {
                cosClient.save2LocalWithoutVersion(requestInfo.key, requestInfo.localPath);
            }
            //requestFunc();
        });

        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();

        activeRequests++;
    }

    COSConfig cosConfig;
    QQueue<QPair<RequestInfo, int>> requestQueue;
    QMutex mutex;
    int activeRequests = 0;
    std::atomic<int> maxConcurrentRequests;
};

#endif // FILEQUEUE_H
