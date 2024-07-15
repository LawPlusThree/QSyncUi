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
        subcard->setStyleSheet("background-color: red;");
        cardMap[subcard]=card;
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

void HistoryviewCardProxy::addHistoryviewCard(QString filename,QString cloudname)
{
    HistoryViewCard*card=new HistoryViewCard(filename,cloudname);
    addHistoryviewCard(card);
}

void HistoryviewCardProxy::addSubCard(QString filename,QString versionID,quint64 datasize,QString bindtime)
{
    QMapIterator<SubCardProxy*,HistoryViewCard*> i(cardMap);
    while(i.hasNext())
    {
        i.next();
        HistoryViewCard*card=i.value();
        if(card->filename->text()==filename)
        {
            SubCardProxy*subcard=i.key();
            subcard->addSubCard(versionID,datasize,bindtime);
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

void SubCardProxy::addSubCard(QString versionID,quint64 datasize,QString bindtime)
{
    SubCard*card=new SubCard(versionID,datasize,bindtime);
    if(card&&parentWidget)
    {
        cardVector.push_back(card);
        subLayout->addWidget(card);
        subLayout->setAlignment(Qt::AlignTop);
    }
}
