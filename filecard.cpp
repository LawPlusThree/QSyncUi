#include "filecard.h"

#include<QVBoxLayout>
#include"ElaCheckBox.h"
#include"ElaIconButton.h"
#include"ElaProgressBar.h"
FileCard::FileCard(QString f, QString d,QString s,QString p)
{
    filename=new ElaText(f);
    datasize=new ElaText(d);
    speed=new ElaText(s);
    progress=new ElaText(p);

    _checkBox = new ElaCheckBox(filename->text(), this);
    QVBoxLayout*checkBoxArea=new QVBoxLayout();
    checkBoxArea->addWidget(_checkBox,0,Qt::AlignCenter);

    datasize->setTextSize(16);
    QVBoxLayout*dataSizeArea=new QVBoxLayout();
    dataSizeArea->addWidget(datasize,0,Qt::AlignCenter);

    speed->setTextSize(16);
    QVBoxLayout*speedArea=new QVBoxLayout();
    speedArea->addWidget(speed,0,Qt::AlignCenter);

    proBar = new ElaProgressBar(this);
    proBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    proBar->setFixedSize(100,15);
    proBar->setMinimum(0);
    proBar->setMaximum(0);
    progress->setTextSize(16);
    QVBoxLayout*proBarArea=new QVBoxLayout();
    proBarArea->addWidget(progress,0,Qt::AlignCenter);
    proBarArea->addWidget(proBar,0,Qt::AlignCenter);
    proBarArea->setAlignment(Qt::AlignCenter);

    modifyBtn=new ElaIconButton(ElaIconType::CalendarLinesPen);
    pauseBtn=new ElaIconButton(ElaIconType::Pause);
    relieveBtn=new ElaIconButton(ElaIconType::Xmark);
    modifyBtn->setFixedSize(30,30);
    pauseBtn->setFixedSize(30,30);
    relieveBtn->setFixedSize(30,30);

    QHBoxLayout*actionArea=new QHBoxLayout();
    actionArea->addWidget(modifyBtn);
    actionArea->addWidget(pauseBtn);
    actionArea->addWidget(relieveBtn);
    actionArea->setAlignment(Qt::AlignCenter);

    QHBoxLayout*FileCardArea=new QHBoxLayout(this);
    FileCardArea->addLayout(checkBoxArea,Qt::AlignCenter);
    FileCardArea->addLayout(dataSizeArea,Qt::AlignCenter);
    FileCardArea->addLayout(speedArea,Qt::AlignCenter);
    FileCardArea->addLayout(proBarArea,Qt::AlignCenter);
    FileCardArea->addLayout(actionArea,Qt::AlignCenter);
    FileCardArea->addStretch();

}
