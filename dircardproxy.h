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

    void addDirCard(DirCard *card, const QString &id);
    void addDirCard(QString filename,QString datasize,QString bindtime,const QString &id);
    void removeDirCard(const QString &id);
    void removeChecked();
    QVBoxLayout*filesLayout;

private:
    QMap<QString, DirCard*> cardMap;
    QWidget *parentWidget;
};

#endif // DIRCARDPROXY_H
