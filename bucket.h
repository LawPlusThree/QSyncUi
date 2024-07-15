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

struct Version{
    QString key;
    QString versionId;
    bool isLatest;
    QDateTime lastModified;
    QString eTag;
    int size;
    QString storageClass;
    QString storageTier;
    Owner owner;
    Version(){}
};

class VersionResult{
public:
    QString encodingType;
    QString name;
    QString prefix;
    QString keyMarker;
    QString versionIdMarker;
    int maxKeys;
    bool isTruncated=false;
    QString nextKeyMarker;
    QString nextVersionIdMarker;
    QString delimiter;
    QVector<QString> commonPrefixes;
    QVector<Version> versions;
    QVector<Version> deleteMarkers;
    VersionResult(){}
};


#endif // BUCKET_H
