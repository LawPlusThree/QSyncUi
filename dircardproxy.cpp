#include "dircardproxy.h"

#include"DirCard.h"
#include<QVBoxLayout>

DirCardProxy::DirCardProxy(QWidget *parent) : QWidget(parent) {
    parentWidget = qobject_cast<QWidget*>(parent);
    filesLayout=new QVBoxLayout(this);
}

DirCardProxy::~DirCardProxy() {
    cardMap.clear();
}

void DirCardProxy::addDirCard(DirCard *card, const int &id) {
    if (card && parentWidget && !cardMap.contains(id)) {
        cardMap[id] = card;
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}

void DirCardProxy::addDirCard(QString filename,int datasize,QString bindtime,const int &id)
{
    DirCard*card=new DirCard(filename,datasize,bindtime,id);
    if (card && parentWidget && !cardMap.contains(id)) {
        cardMap[id] = card;
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}

void DirCardProxy::removeDirCard(const int &id) {
    if (cardMap.contains(id)) {
        DirCard *card = cardMap.take(id);
        // 在这里可以从UI中移除card
        filesLayout->removeWidget(card);
        card->setParent(nullptr);
        card->deleteLater();
    }
}

void DirCardProxy::removeChecked()
{
    QMapIterator<int, DirCard*> i(cardMap);
    while (i.hasNext())
    {
        i.next();
        int id = i.key();
        DirCard *card = i.value();
        if(card->ischecked())
            removeDirCard(id);
    }
}

void DirCardProxy::modifyDirCard(int datasize,QString bindtime,int id)
{
    QMapIterator<int,DirCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        int Id=i.key();
        DirCard*card=i.value();
        if(Id==id)
        {
            card->modify(datasize,bindtime);
        }
    }
}
