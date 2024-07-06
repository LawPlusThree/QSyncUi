#ifndef FILEFUNC_H
#define FILEFUNC_H

#include <QObject>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>

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
    void fileListUpdated(); // 文件列表更新信号

public:
    // 获取文件信息列表
    QFileInfoList getFileInfoList() const { return fileInfoList; }
};






#endif // FILEFUNC_H
