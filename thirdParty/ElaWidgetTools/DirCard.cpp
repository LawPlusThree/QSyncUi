#include "DirCard.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include<QVBoxLayout>

#include"ElaCheckBox.h"
#include "private/DirCardPrivate.h"
#include"ElaPushButton.h"
#include"ElaToggleButton.h"

DirCard::DirCard(QString f, QString d,QString b)
{

    filename=new ElaText(f);
    datasize=new ElaText(d);
    bindtime=new ElaText(b);

    _checkBox = new ElaCheckBox(filename->text(), this);
    QVBoxLayout*checkBoxArea=new QVBoxLayout();
    checkBoxArea->addWidget(_checkBox,0,Qt::AlignCenter);

    datasize->setTextSize(16);
    QVBoxLayout*dataSizeArea=new QVBoxLayout();
    dataSizeArea->addWidget(datasize,0,Qt::AlignCenter);

    bindtime->setTextSize(16);
    QVBoxLayout*bindTimeArea=new QVBoxLayout();
    bindTimeArea->addWidget(bindtime,0,Qt::AlignCenter);

    //QVBoxLayout*pauseArea=new QVBoxLayout();
    pauseBtn=new ElaToggleButton("暂时停止");
    pauseBtn->setFixedSize(80,30);
    //pauseArea->addWidget(pauseBtn,0,Qt::AlignCenter);

    //QVBoxLayout*relieveArea=new QVBoxLayout();
    relieveBtn=new ElaPushButton("解除绑定");
    relieveBtn->setFixedSize(80,30);
    //relieveArea->addWidget(relieveBtn,0,Qt::AlignCenter);

    QHBoxLayout*prArea=new QHBoxLayout();
    //prArea->addLayout(pauseArea,Qt::AlignCenter);
    //prArea->addLayout(relieveArea,Qt::AlignCenter);
    prArea->addWidget(pauseBtn);
    prArea->addWidget(relieveBtn);
    prArea->setAlignment(Qt::AlignCenter);

    QHBoxLayout*DirCardArea=new QHBoxLayout(this);
    DirCardArea->addLayout(checkBoxArea,Qt::AlignCenter);
    DirCardArea->addLayout(dataSizeArea,Qt::AlignCenter);
    DirCardArea->addLayout(bindTimeArea,Qt::AlignCenter);
    DirCardArea->addLayout(prArea,Qt::AlignCenter);

    connect(relieveBtn,&ElaPushButton::clicked,this,&DirCard::on_relieveBtn_clicked);
}

void DirCard::on_relieveBtn_clicked()
{
    emit relieve();
}
