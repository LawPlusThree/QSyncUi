#include "historyviewcardproxy.h"
#include"historyviewcard.h"
#include<QVBoxLayout>
#include"ElaWidget.h"
#include"ElaCheckBox.h"
#include"ElaIconButton.h"


HistoryviewCardProxy::HistoryviewCardProxy(QWidget*parent) {
    parentWidget = qobject_cast<QWidget*>(parent);
    filesLayout=new QVBoxLayout(this);
}

HistoryviewCardProxy::~HistoryviewCardProxy()
{
    cardMap.clear();
}

void HistoryviewCardProxy::addHistoryviewCard(HistoryViewCard *card) {
    if (card && parentWidget) {
        SubCardProxy*subcard=new SubCardProxy(this);
        cardMap[subcard]=card;
        subcard->subLayout->setContentsMargins(10,0,10,10);
        filesLayout->addWidget(card);
        filesLayout->addWidget(subcard);
        subcard->setVisible(false);
        filesLayout->setAlignment(Qt::AlignTop);
    }
    connect(card->button, &ElaIconButton::clicked, this, [=]()
            {
        QMapIterator<SubCardProxy*,HistoryViewCard*> i(cardMap);
        while(i.hasNext())
        {
            i.next();
            if(card==i.value())
            {
                SubCardProxy* Sub=i.key();
                if(card->button->getAwesome()==ElaIconType::AngleDown)
                {
                    card->button->setAwesome(ElaIconType::AngleUp);
                    Sub->setVisible(true);
                }
                else
                {
                    card->button->setAwesome(ElaIconType::AngleDown);
                    Sub->setVisible(false);
                }
            }
        }
    });
}

void HistoryviewCardProxy::addHistoryviewCard(QString filename,QString cloudname,QString path)
{
    HistoryViewCard*card=new HistoryViewCard(filename,cloudname,path);
    addHistoryviewCard(card);
}


bool HistoryviewCardProxy::isExist(QString filename)
{
    QMapIterator<SubCardProxy*,HistoryViewCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        HistoryViewCard*card=i.value();
        if(card->fullText.compare(filename)==0)
        {
            return true;
        }
    }
    return false;
}

void HistoryviewCardProxy::removeAll()
{
    QMapIterator<SubCardProxy*,HistoryViewCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        HistoryViewCard*card=i.value();
        clearAllSub(card->fullText);
        filesLayout->removeWidget(card);
        card->setParent(nullptr);
        card->deleteLater();
    }
}

void HistoryviewCardProxy::clearAllSub(QString filename)
{
    QMapIterator<SubCardProxy*,HistoryViewCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        HistoryViewCard*card=i.value();
        if(card->fullText.compare(filename)==0)
        {
            auto subCardProxy=i.key();
            for(auto&x:subCardProxy->cardVector)
            {
                subCardProxy->subLayout->removeWidget(x);
                x->setParent(nullptr);
                x->deleteLater();
            }
            subCardProxy->cardVector.clear();
        }
    }
}

void HistoryviewCardProxy::addSubCard(QString filename,QString versionID,quint64 datasize,QString bindtime)
{
    QMapIterator<SubCardProxy*,HistoryViewCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        HistoryViewCard*card=i.value();
        if(card->fullText.compare(filename)==0)
        {
            SubCardProxy*subcard=i.key();
            disconnect(subcard, &SubCardProxy::message, this, 0);
            SubCard* thisCard=subcard->addSubCard(versionID,datasize,bindtime);
            connect(thisCard->rollback,&ElaPushButton::clicked,[=](){
                emit Message(versionID,card->cloudName,filename,card->path);
            });
            return;
        }
    }
}

SubCardProxy::SubCardProxy(QWidget*parent) {
    parentWidget = qobject_cast<QWidget*>(parent);
    subLayout=new QVBoxLayout(this);
}

SubCardProxy::~SubCardProxy()
{
    cardVector.clear();
}

SubCard *SubCardProxy::addSubCard(QString versionID, quint64 datasize, QString bindtime)
{
    SubCard*card=new SubCard(versionID,datasize,bindtime);
    if(card&&parentWidget)
    {
        cardVector.push_back(card);
        subLayout->addWidget(card);
        subLayout->setAlignment(Qt::AlignTop);
    }
    return card;
}

