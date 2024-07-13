#ifndef BUCKET_H
#define BUCKET_H

#include <QVector>
#include <QString>
#include <QDateTime>
// 所有者类
struct Owner {
    QString id;
    QString displayName;
};
// 内容类
struct Content {
    QString key;
    QDateTime lastModified;
    QString eTag;
    int size;
    Owner owner;
    QString storageClass;
    QString storageTier;
    QString restoreStatus;

    Content(){}
};

class Bucket {
public:
    QString name;
    QString encodingType;
    QString prefix;
    QString marker;
    int maxKeys;
    QString delimiter;
    bool isTruncated=false;
    QString nextMarker;
    QVector<QString> commonPrefixes;
    QVector<Content> contents;

    Bucket(){}

};
#endif // BUCKET_H
