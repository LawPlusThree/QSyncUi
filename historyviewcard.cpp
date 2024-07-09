#include "historyviewcard.h"
#include<QVBoxLayout>
#include"ElaCheckBox.h"


HistoryViewCard::HistoryViewCard(QString f, QString d,QString t)
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

    timeArea->addWidget(time,0,Qt::AlignCenter);

    rollback = new ElaPushButton(this);
    rollback->setFixedSize(100,40);
    rollback->setText("回滚到此版本");
    QVBoxLayout*rollbackArea = new QVBoxLayout();
    rollbackArea->addWidget(rollback,0,Qt::AlignCenter);

    QHBoxLayout*FileCardArea=new QHBoxLayout(this);
    FileCardArea->addLayout(checkBoxArea,Qt::AlignCenter);
    FileCardArea->addLayout(dataSizeArea,Qt::AlignCenter);
    FileCardArea->addLayout(timeArea,Qt::AlignCenter);
    FileCardArea->addLayout(rollbackArea,Qt::AlignCenter);
    FileCardArea->addStretch();
}
