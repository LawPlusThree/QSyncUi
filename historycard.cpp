#include "historycard.h"

#include<QVBoxLayout>
#include"ElaCheckBox.h"
#include"ElaIcon.h"
#include"ElaIconButton.h"

HistoryCard::HistoryCard(QString f, QString d,QString t,bool u)
{
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

    ElaIconButton* checkIcon=new ElaIconButton(ElaIconType::Check);
    checkIcon->setFixedSize(30,30);
    checkIcon->setEnabled(false); // 禁用按钮
    QVBoxLayout*bingoArea=new QVBoxLayout();
    bingoArea->addWidget(checkIcon,0,Qt::AlignCenter);

    QHBoxLayout*HistoryCardArea=new QHBoxLayout(this);
    HistoryCardArea->addLayout(checkBoxArea,Qt::AlignCenter);
    HistoryCardArea->addLayout(dataSizeArea,Qt::AlignCenter);
    HistoryCardArea->addLayout(timeArea,Qt::AlignCenter);
    HistoryCardArea->addLayout(bingoArea,Qt::AlignCenter);
    HistoryCardArea->addStretch();
}
