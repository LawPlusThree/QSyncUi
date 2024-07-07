#ifndef FILEFUNC_H
#define FILEFUNC_H

#include <QObject>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <QDateTime>
#include <QString>

class Filefunc : public QObject
{
    Q_OBJECT

public:
    explicit Filefunc(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    // 读取文件夹及其子文件夹中的所有文件
    void readDirectory(const QString &path);

private slots:
    // 递归遍历文件夹和子文件夹
    void recursiveRead(const QString &path);

private:
    // 存储文件信息列表
    QFileInfoList fileInfoList;

signals:
    void fileListUpdated(const QString &path, const QFileInfoList &list);

public:
    // 获取文件信息列表
    QFileInfoList getFileInfoList() const { return fileInfoList; }
    bool comparetime(QDateTime localtime,QString cloudtimestr);//比较两端文件时间,若本地文件时间新则返回真
};






#endif // FILEFUNC_H
