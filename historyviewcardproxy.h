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
    void addHistoryviewCard(QString filename,QString cloudname);
    void addSubCard(QString filename,QString versionID,quint64 datasize,QString bindtime);
    QVBoxLayout*filesLayout;

private:
    QMap<SubCardProxy*,HistoryViewCard*>cardMap;
    //QMap<HistoryViewCard*,ElaWidget*>cardMap;
    QWidget *parentWidget;
    friend class HistoryViewPage;
};

#endif // HISTORYVIEWCARDPROXY_H
