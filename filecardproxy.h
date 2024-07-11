#ifndef FILECARDPROXY_H
#define FILECARDPROXY_H

#include <QMap>
#include <QWidget>

class FileCard;
class QVBoxLayout;
class FileCardProxy : public QWidget {
    Q_OBJECT
public:
    explicit FileCardProxy(QWidget *parent = nullptr);
    ~FileCardProxy() override;

    void addFileCard(FileCard *card, const int &id);
    void addFileCard(QString filename,qint64 datasize,double speed,int progress,const int &id);
    void removeFileCard(const int &id);
    //void modify(int d,int s,int p,int Id);
    void modify(qint64 totalsize,qint64 currentsize,int id);
    void processing(int p,int Id);
    int totalprogress();
    QVBoxLayout*filesLayout;

private:
    QMap<int, FileCard*> cardMap;
    QWidget *parentWidget;
    friend class SyncingPage;
};


#endif // FILECARDPROXY_H
