#include "historycard.h"

#include<QVBoxLayout>
#include"ElaCheckBox.h"

HistoryCard::HistoryCard(QString f, QString d,QString t,bool u)
{
    setMaximumHeight(25);

    filename=new ElaText(f);
    datasize=new ElaText(d);
    time=new ElaText(t);

    _checkBox = new ElaCheckBox(filename->text(), this);
    QVBoxLayout*checkBoxArea=new QVBoxLayout();
    checkBoxArea->addWidget(_checkBox,0,Qt::AlignCenter);

    datasize->setTextSize(16);
    QVBoxLayout*dataSizeArea=new QVBoxLayout();
    dataSizeArea->addWidget(datasize,0,Qt::AlignCenter);

    time->setTextSize(16);
    QVBoxLayout*timeArea=new QVBoxLayout();
    ElaText*text;
    if(u)
        text=new ElaText("上传于");
    else
        text=new ElaText("下载于");
    text->setTextSize(16);
    timeArea->addWidget(text,0,Qt::AlignCenter);
    timeArea->addWidget(time,0,Qt::AlignCenter);

    ElaText*bingoText=new ElaText("√");
    bingoText->setTextSize(16);
    QVBoxLayout*bingoArea=new QVBoxLayout();
    bingoArea->addWidget(bingoText,0,Qt::AlignCenter);

    QHBoxLayout*HistoryCardArea=new QHBoxLayout(this);
    HistoryCardArea->addLayout(checkBoxArea,Qt::AlignCenter);
    HistoryCardArea->addLayout(dataSizeArea,Qt::AlignCenter);
    HistoryCardArea->addLayout(timeArea,Qt::AlignCenter);
    HistoryCardArea->addLayout(bingoArea,Qt::AlignCenter);
    HistoryCardArea->addStretch();
}
