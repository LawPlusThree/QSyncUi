#include "filecardproxy.h"

#include"FileCard.h"
#include<QVBoxLayout>

FileCardProxy::FileCardProxy(QWidget *parent) : QWidget(parent) {
    parentWidget = qobject_cast<QWidget*>(parent);
    filesLayout=new QVBoxLayout(this);
}

FileCardProxy::~FileCardProxy() {
    cardMap.clear();
}

void FileCardProxy::addFileCard(FileCard *card, const int &id) {
    if (card && parentWidget && !cardMap.contains(id)) {
        cardMap[id] = card;
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}

void FileCardProxy::addFileCard(QString filename,int datasize,int speed,int progress,const int &id)
{
    FileCard*card=new FileCard(filename,datasize,speed,progress,id);
    if (card && parentWidget && !cardMap.contains(id)) {
        cardMap[id] = card;
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}

void FileCardProxy::removeFileCard(const int &id) {
    if (cardMap.contains(id)) {
        FileCard *card = cardMap.take(id);
        // 在这里可以从UI中移除card
        filesLayout->removeWidget(card);
        card->setParent(nullptr);
        card->deleteLater();
    }
}

void FileCardProxy::modify(int d,int s,int p,int Id)
{
    QMapIterator<int,FileCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        int id=i.key();
        FileCard*card=i.value();
        if(id==Id)
            card->modify(d,s,p);
    }
}

void FileCardProxy::processing(int p,int Id)
{
    QMapIterator<int,FileCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        int id=i.key();
        FileCard*card=i.value();
        if(id==Id)
            card->processing(p);
    }
}

int FileCardProxy::totalprogress()
{
    int total=0;
    int x=0;
    QMapIterator<int,FileCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        int id=i.key();
        FileCard*card=i.value();
        total+=card->progress;
        x++;
    }
    int progress;
    if(x!=0)
        progress=total/x;
    else
        progress=0;
    return progress;
}
