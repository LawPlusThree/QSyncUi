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
    void addDirCard(QString filename,QString cloudname,quint64 datasize,QString bindtime,int syncStatus,const int &id);
    void removeDirCard(const int &id);
    void removeChecked();
    void modifyDirCard(quint64 datasize,QString bindtime,int id);
    QVBoxLayout*filesLayout;
    friend class DirCard;

private:
    QMap<int, DirCard*> cardMap;
    QWidget *parentWidget;
    friend class FileManagePage;

signals:
    void checkBoxToggled(bool checked); // 当DirCard中的复选框被勾选时发出的信号
    void removeCard();

private slots:
    void onCheckBoxStateChanged(int state); // 处理复选框状态变化的槽
};

#endif // DIRCARDPROXY_H
