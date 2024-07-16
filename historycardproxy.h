#ifndef HISTORYCARDPROXY_H
#define HISTORYCARDPROXY_H

#include <QVector>
#include <QWidget>

class HistoryCard;
class QVBoxLayout;
class HistoryCardProxy : public QWidget {
    Q_OBJECT
public:
    explicit HistoryCardProxy(QWidget *parent = nullptr);
    ~HistoryCardProxy() override;

    void addHistoryCard(HistoryCard *card);
    void addHistoryCard(QString filename,QString datasize,QString time,bool upif);
    void removeHistoryCard(QString filename);
    void removeAll();
    QVBoxLayout*filesLayout;

private:
    QVector<HistoryCard*>cardVector;
    QWidget *parentWidget;
    friend class HistorysyncPage;
};
#endif // HISTORYCARDPROXY_H
