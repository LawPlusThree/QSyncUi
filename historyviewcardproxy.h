#ifndef HISTORYVIEWCARDPROXY_H
#define HISTORYVIEWCARDPROXY_H

#include <QVector>
#include <QWidget>

class HistoryViewCard;
class QVBoxLayout;
class HistoryviewCardProxy:public QWidget{
    Q_OBJECT
public:
    explicit HistoryviewCardProxy(QWidget *parent = nullptr);
    ~HistoryviewCardProxy() override;

    void addHistoryviewCard(HistoryViewCard *card);
    void addHistoryviewCard(QString filename,QString datasize,QString bindtime);
    QVBoxLayout*filesLayout;

private:
    QVector<HistoryViewCard*>cardVector;
    QWidget *parentWidget;
    friend class HistoryViewPage;
};

#endif // HISTORYVIEWCARDPROXY_H
