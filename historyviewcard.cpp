#include "historyviewcard.h"
#include<QVBoxLayout>
#include"ElaCheckBox.h"
#include"ElaIconButton.h"

HistoryViewCard::HistoryViewCard(QString f, QString d,QString t)
{
    filename=new ElaText(f);
    datasize=new ElaText(d);
    time=new ElaText(t);

    datasize->setAlignment(Qt::AlignCenter);
    time->setAlignment(Qt::AlignCenter);

    _checkBox = new ElaCheckBox(filename->text(), this);
    _checkBox->setFixedSize(25,25);
    QVBoxLayout*checkBoxArea=new QVBoxLayout();
    checkBoxArea->addWidget(_checkBox,0,Qt::AlignCenter);

    filename->setTextSize(16);
    filename->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    QWidget* filenameWidget = new QWidget();
    filenameWidget->setWindowFlags(Qt::FramelessWindowHint);
    filenameWidget->setAttribute(Qt::WA_TranslucentBackground);
    filenameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* filenameArea = new QVBoxLayout(filenameWidget);
    filenameArea->addWidget(filename, 0, Qt::AlignLeft);

    datasize->setTextSize(16);
    datasize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    datasize->setFixedSize(100,20);
    QVBoxLayout*dataSizeArea=new QVBoxLayout();
    dataSizeArea->addWidget(datasize,0,Qt::AlignCenter);

    time->setTextSize(16);
    time->setFixedWidth(160);
    QVBoxLayout*timeArea=new QVBoxLayout();

    timeArea->addWidget(time,0,Qt::AlignCenter);

    rollback = new ElaPushButton(this);
    rollback->setFixedSize(100,40);
    rollback->setText("回滚到此版本");
    QVBoxLayout*rollbackArea = new QVBoxLayout();
    rollbackArea->addWidget(rollback,0,Qt::AlignCenter);

    QHBoxLayout*FileCardArea=new QHBoxLayout(this);
    FileCardArea->addLayout(checkBoxArea);
    FileCardArea->addWidget(filenameWidget);
    FileCardArea->addLayout(dataSizeArea);
    FileCardArea->addLayout(timeArea);
    FileCardArea->addLayout(rollbackArea);
    FileCardArea->setStretchFactor(checkBoxArea,25);
    FileCardArea->setStretchFactor(filenameWidget,500);
    FileCardArea->setStretchFactor(dataSizeArea,100);
    FileCardArea->setStretchFactor(timeArea,160);
    FileCardArea->setStretchFactor(rollbackArea,100);
}
