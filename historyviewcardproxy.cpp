#include "historyviewcardproxy.h"
#include"historyviewcard.h"
#include<QVBoxLayout>

HistoryviewCardProxy::HistoryviewCardProxy(QWidget*parent) {
    parentWidget = qobject_cast<QWidget*>(parent);
    filesLayout=new QVBoxLayout(this);
}

HistoryviewCardProxy::~HistoryviewCardProxy()
{
    cardVector.clear();
}

void HistoryviewCardProxy::addHistoryviewCard(HistoryViewCard *card) {
    if (card && parentWidget) {
        cardVector.push_back(card);
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}

void HistoryviewCardProxy::addHistoryviewCard(QString filename,QString datasize,QString bindtime)
{
    HistoryViewCard*card=new HistoryViewCard(filename,datasize,bindtime);
    if (card && parentWidget) {
        cardVector.push_back(card);
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}
