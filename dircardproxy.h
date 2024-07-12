#ifndef DIRCARDPROXY_H
#define DIRCARDPROXY_H

#include <QMap>
#include <QWidget>

class DirCard;
class QVBoxLayout;
class DirCardProxy : public QWidget {
    Q_OBJECT
public:
    explicit DirCardProxy(QWidget *parent = nullptr);
    ~DirCardProxy() override;

    void addDirCard(DirCard *card, const int &id);
    void addDirCard(QString filename,quint64 datasize,QString bindtime,const int &id);
    void removeDirCard(const int &id);
    void removeChecked();
    void modifyDirCard(quint64 datasize,QString biindtime,int id);
    QVBoxLayout*filesLayout;

private:
    QMap<int, DirCard*> cardMap;
    QWidget *parentWidget;
    friend class FileManagePage;
};

#endif // DIRCARDPROXY_H
