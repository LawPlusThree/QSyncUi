#include "DirCard.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include<QVBoxLayout>

#include"ElaCheckBox.h"
#include "private/DirCardPrivate.h"
#include"ElaPushButton.h"
#include"ElaToggleButton.h"

DirCard::DirCard(QString f, int d,QString b,int Id)
{
    filename=new ElaText(f);

    QString size;
    QString dataStr;
    double data=d;
    data/=1000;
    if(data<1000)
    {
        dataStr=QString::number(data,'f',1);
        size=dataStr+"KB";
    }
    else
    {
        data/=1000;
        if(data<1000)
        {
            dataStr=QString::number(data,'f',1);
            size=dataStr+"MB";
        }
        else
        {
            data/=1000;
            dataStr=QString::number(data,'f',1);
            size=dataStr+"GB";
        }
    }
    datasize = new ElaText(size, this);

    bindtime=new ElaText(b);
    id=Id;

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
    qDebug()<<"发出前的id"<<id;
    emit relieve(id);
}

bool DirCard::ischecked()
{
    return _checkBox->isChecked();
}

void DirCard::modify(int d,QString b)
{
    QString size;
    QString dataStr;
    double data=d;
    data/=1000;
    if(data<1000)
    {
        dataStr=QString::number(data,'f',1);
        size=dataStr+"KB";
    }
    else
    {
        data/=1000;
        if(data<1000)
        {
            dataStr=QString::number(data,'f',1);
            size=dataStr+"MB";
        }
        else
        {
            data/=1000;
            dataStr=QString::number(data,'f',1);
            size=dataStr+"GB";
        }
    }
    datasize->setText(size);
    bindtime->setText(b);
}
