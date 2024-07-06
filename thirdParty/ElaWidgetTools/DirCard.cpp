#include "DirCard.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include<QVBoxLayout>

#include"ElaCheckBox.h"
#include "private/DirCardPrivate.h"
#include"ElaText.h"
#include"ElaPushButton.h"
#include"ElaToggleButton.h"

DirCard::DirCard(QString filename, QString datasize,QString bingtime)
{
    //ElaScrollPageArea*DirCardArea=new ElaScrollPageArea(this);
    this->setMaximumHeight(25);
    int DirCardWidth=this->width();

    QHBoxLayout*DirCardLayout=new QHBoxLayout(this);

    _checkBox = new ElaCheckBox(filename, this);
    _checkBox->setFont(QFont("",10));

    ElaText* dataSize = new ElaText(datasize, this);
    ElaText*text=new ElaText("绑定于",this);
    ElaText*bindTime=new ElaText(bingtime,this);

    dataSize->setTextSize(10);
    text->setTextSize(10);
    bindTime->setTextSize(10);

    DirCardLayout->addWidget(_checkBox);

    int index1=DirCardLayout->count();
    DirCardLayout->insertSpacing(index1,DirCardWidth/6);

    DirCardLayout->addWidget(dataSize);

    int index2=DirCardLayout->count();
    DirCardLayout->insertSpacing(index2,DirCardWidth/4*3);

    DirCardLayout->addWidget(text);
    DirCardLayout->addWidget(bindTime);

    int index3=DirCardLayout->count();
    DirCardLayout->insertSpacing(index3,DirCardWidth/5);

    ElaToggleButton*pause=new ElaToggleButton("暂时停止");
    pause->setFixedSize(70,25);
    pause->setFont(QFont("",10));

    ElaPushButton*relieve=new ElaPushButton("解除绑定");
    relieve->setFixedSize(70,25);
    relieve->setFont(QFont("",10));

    DirCardLayout->addWidget(pause);
    DirCardLayout->addWidget(relieve);
    DirCardLayout->addStretch();
}

DirCard::~DirCard() {}
