#include "filecardproxy.h"

#include"FileCard.h"
#include<QVBoxLayout>
#include"ElaCheckBox.h"

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
        connect(card->getCheckBox(), &ElaCheckBox::stateChanged, this, &FileCardProxy::onCheckBoxStateChanged);
    }
}

void FileCardProxy::addFileCard(QString filename,quint64 datasize,double speed,int progress,int syncStatus,const int &id)
{
    FileCard*card=new FileCard(filename,datasize,speed,progress,syncStatus,id);
    if (card && parentWidget && !cardMap.contains(id)) {
        cardMap[id] = card;
        filesLayout->addWidget(card);
        filesLayout->setAlignment(Qt::AlignTop);
    }
}

void FileCardProxy::removeFileCard(const int &id) {
    if (cardMap.contains(id)) {
        FileCard *card = cardMap.take(id);
        card->processing(100);
        // 在这里可以从UI中移除card
        filesLayout->removeWidget(card);
        card->setParent(nullptr);
        card->deleteLater();
    }
}

/*void FileCardProxy::modify(int d,int s,int p,int Id)
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
}*/

void FileCardProxy::modify(quint64 totalsize,quint64 currentsize,int Id)
{
    if(cardMap.find(Id)==cardMap.end())
        return;
    auto thisCard=cardMap[Id];
    thisCard->modify(totalsize,currentsize);
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

void FileCardProxy::onCheckBoxStateChanged(int state)
{
    emit checkBoxToggled(state == Qt::Checked); // 发出信号，参数为复选框是否被勾选
}
