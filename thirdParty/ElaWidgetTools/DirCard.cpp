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
    this->setMaximumHeight(25);

    filename=new ElaText(f);
    datasize=new ElaText(d);
    bindtime=new ElaText(b);
    QFont font("宋体",10);
    font.setBold(true);

    _checkBox = new ElaCheckBox(filename->text(), this);
    _checkBox->setFont(font);
    QVBoxLayout*checkBoxArea=new QVBoxLayout();
    checkBoxArea->addWidget(_checkBox,0,Qt::AlignCenter);

    datasize->setTextSize(12);
    datasize->setFont(font);
    QVBoxLayout*dataSizeArea=new QVBoxLayout();
    dataSizeArea->addWidget(datasize,0,Qt::AlignCenter);

    bindtime->setTextSize(12);
    bindtime->setFont(font);
    QVBoxLayout*bindTimeArea=new QVBoxLayout();
    ElaText*text=new ElaText("绑定于",this);
    text->setTextSize(12);
    text->setFont(font);
    bindTimeArea->addWidget(text,0,Qt::AlignCenter);
    bindTimeArea->setSpacing(0);
    bindTimeArea->addWidget(bindtime,0,Qt::AlignCenter);

    QVBoxLayout*pauseArea=new QVBoxLayout();
    ElaToggleButton*pauseBtn=new ElaToggleButton("暂时停止");
    pauseBtn->setFixedSize(70,25);
    pauseBtn->setFont(font);
    pauseArea->addWidget(pauseBtn,0,Qt::AlignCenter);

    QVBoxLayout*relieveArea=new QVBoxLayout();
    ElaPushButton*relieveBtn=new ElaPushButton("解除绑定");
    relieveBtn->setFixedSize(70,25);
    relieveBtn->setFont(font);
    relieveArea->addWidget(relieveBtn,0,Qt::AlignCenter);

    QHBoxLayout*prArea=new QHBoxLayout();
    prArea->addLayout(pauseArea,Qt::AlignCenter);
    prArea->addLayout(relieveArea,Qt::AlignCenter);

    QHBoxLayout*DirCardArea=new QHBoxLayout(this);
    DirCardArea->addLayout(checkBoxArea,Qt::AlignCenter);
    DirCardArea->addLayout(dataSizeArea,Qt::AlignCenter);
    DirCardArea->addLayout(bindTimeArea,Qt::AlignCenter);
    DirCardArea->addLayout(prArea,Qt::AlignCenter);
}

DirCard::~DirCard() {}
