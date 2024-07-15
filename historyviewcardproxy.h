#ifndef HISTORYVIEWCARDPROXY_H
#define HISTORYVIEWCARDPROXY_H

#include <QVector>
#include <QWidget>

class HistoryViewCard;
class SubCard;
class QVBoxLayout;
class ElaWidget;

class SubCardProxy:public QWidget{
    Q_OBJECT
public:
    explicit SubCardProxy(QWidget*parent =nullptr);
    ~SubCardProxy() override;

    void addSubCard(QString versionID,quint64 datasize,QString bindtime);
    QVBoxLayout*subLayout;

signals:
    void message(QString versionID);

private:
    QVector<SubCard*>cardVector;
    QWidget *parentWidget;
    friend class HistoryViewPage;
};

class HistoryviewCardProxy:public QWidget{
    Q_OBJECT
public:
    explicit HistoryviewCardProxy(QWidget *parent = nullptr);
    ~HistoryviewCardProxy() override;

    void addHistoryviewCard(HistoryViewCard *card);
    void addHistoryviewCard(QString filename,QString cloudname,QString path);
    void addSubCard(QString filename,QString versionID,quint64 datasize,QString bindtime);
    QVBoxLayout*filesLayout;


signals:
    void Message(QString versionID,QString cloudname,QString filename,QString path);

private:
    QMap<SubCardProxy*,HistoryViewCard*>cardMap;
    QWidget *parentWidget;
    friend class HistoryViewPage;
};

#endif // HISTORYVIEWCARDPROXY_H
