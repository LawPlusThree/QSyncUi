#include "historycardproxy.h"
#include"historycard.h"
#include<QVBoxLayout>

HistoryCardProxy::HistoryCardProxy(QWidget *parent) : QWidget(parent) {
    parentWidget = qobject_cast<QWidget*>(parent);
    filesLayout=new QVBoxLayout(this);
}

HistoryCardProxy::~HistoryCardProxy() {
    cardVector.clear();
}

void HistoryCardProxy::addHistoryCard(HistoryCard *card) {
    if (card && parentWidget) {
        cardVector.push_back(card);
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}

void HistoryCardProxy::addHistoryCard(QString filename,QString datasize,QString time,bool upif)
{
    HistoryCard*card=new HistoryCard(filename,datasize,time,upif);
    if (card && parentWidget) {
        cardVector.push_back(card);
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}

void HistoryCardProxy::removeHistoryCard(QString filename)
{
    for(auto &x:cardVector)
    {
        if(x->fullText==filename)
        {
            filesLayout->removeWidget(x);
            x->setParent(nullptr);
            x->deleteLater();
        }
    }
}
