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
    void addDirCard(QString filename,int datasize,QString bindtime,const int &id);
    void removeDirCard(const int &id);
    void removeChecked();
    void modifyDirCard(int datasize,QString biindtime,int id);
    QVBoxLayout*filesLayout;

private:
    QMap<int, DirCard*> cardMap;
    QWidget *parentWidget;
};

#endif // DIRCARDPROXY_H
