#include "historyviewcard.h"
#include<QVBoxLayout>
#include"ElaCheckBox.h"
#include"ElaIconButton.h"
#include<QPainter>
#include"ElaApplication.h"

HistoryViewCard::HistoryViewCard(QString file,QString cloud)
{
    filename=new ElaText(file);
    cloudname=new ElaText(cloud);

    fullText=file;
    cfullText=cloud;

    filename->setWordWrap(false);
    cloudname->setWordWrap(false);

    filename->setTextSize(16);
    filename->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    QWidget* filenameWidget = new QWidget();
    filenameWidget->setWindowFlags(Qt::FramelessWindowHint);
    filenameWidget->setAttribute(Qt::WA_TranslucentBackground);
    filenameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* filenameArea = new QVBoxLayout(filenameWidget);
    filenameArea->addWidget(filename, 0, Qt::AlignLeft);

    cloudname->setTextSize(16);
    cloudname->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    QWidget* cloudnameWidget = new QWidget();
    cloudnameWidget->setWindowFlags(Qt::FramelessWindowHint);
    cloudnameWidget->setAttribute(Qt::WA_TranslucentBackground);
    cloudnameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* cloudnameArea = new QVBoxLayout(cloudnameWidget);
    cloudnameArea->addWidget(cloudname, 0, Qt::AlignLeft);

    button=new ElaIconButton(ElaIconType::AngleDown,this);
    button->setFixedSize(35,35);
    QVBoxLayout*buttonArea=new QVBoxLayout();
    buttonArea->addWidget(button,0,Qt::AlignCenter);

    ElaText* catalogueText0 = new ElaText("", this);
    catalogueText0->setTextSize(16);
    catalogueText0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText0->setFixedSize(25,20);
    catalogueText0->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea0 = new QVBoxLayout();
    catalogueArea0->addWidget(catalogueText0, 0, Qt::AlignCenter);

    QHBoxLayout*FileCardArea=new QHBoxLayout(this);
    FileCardArea->addLayout(catalogueArea0);
    FileCardArea->addWidget(filenameWidget);
    FileCardArea->addWidget(cloudnameWidget);
    FileCardArea->addLayout(buttonArea);

    FileCardArea->setStretchFactor(catalogueArea0,25);
    FileCardArea->setStretchFactor(filenameWidget,430);
    FileCardArea->setStretchFactor(cloudnameWidget,375);
    FileCardArea->setStretchFactor(buttonArea,35);

    filenameWidget->setStyleSheet("QToolTip { color: #5C5C5C; background-color: #F9F9F9; border: 1px solid #808080; border-radius: 3px; }");
    cloudnameWidget->setStyleSheet("QToolTip { color: #5C5C5C; background-color: #F9F9F9; border: 1px solid #808080; border-radius: 3px; }");
}

SubCard::SubCard(QString ID,quint64 d,QString time)
{
    versionID=new ElaText(ID);
    datasize=d;
    bindtime=new ElaText(time);

    QString size;
    QString dataStr;
    double data=d;
    data/=1024;
    if(data<1024)
    {
        dataStr=QString::number(data,'f',1);
        size=dataStr+"KB";
    }
    else
    {
        data/=1024;
        if(data<1024)
        {
            dataStr=QString::number(data,'f',1);
            size=dataStr+"MB";
        }
        else
        {
            data/=1024;
            dataStr=QString::number(data,'f',1);
            size=dataStr+"GB";
        }
    }
    ElaText *dataSize = new ElaText(size, this);

    versionID->setWordWrap(false);
    dataSize->setWordWrap(false);
    bindtime->setWordWrap(false);

    versionID->setTextSize(16);
    versionID->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    versionID->setFixedSize(100,20);
    QVBoxLayout*versionIDArea=new QVBoxLayout();
    versionIDArea->addWidget(versionID,0,Qt::AlignCenter);

    dataSize->setTextSize(16);
    dataSize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    dataSize->setFixedSize(100,20);
    QVBoxLayout*dataSizeArea=new QVBoxLayout();
    dataSizeArea->addWidget(dataSize,0,Qt::AlignCenter);

    bindtime->setTextSize(16);
    bindtime->setFixedWidth(200);
    QVBoxLayout*bindtimeArea=new QVBoxLayout();
    bindtimeArea->addWidget(bindtime,0,Qt::AlignCenter);

    rollback=new ElaPushButton("下载");
    rollback->setFixedWidth(100);
    QHBoxLayout *rollbackArea=new QHBoxLayout();
    rollbackArea->addWidget(rollback);

    QHBoxLayout*subcardLayout=new QHBoxLayout(this);
    subcardLayout->addLayout(bindtimeArea);
    subcardLayout->addLayout(versionIDArea);
    subcardLayout->addLayout(dataSizeArea);
    subcardLayout->addLayout(rollbackArea);
    subcardLayout->setStretchFactor(bindtimeArea,245);
    subcardLayout->setStretchFactor(versionIDArea,245);
    subcardLayout->setStretchFactor(dataSizeArea,245);
    subcardLayout->setStretchFactor(rollback,100);
}


void SubCard::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    auto themeMode=ElaApplication::getInstance()->getThemeMode();
    if (themeMode == ElaApplicationType::Light)
    {
        painter.setPen(QPen(QColor(0xDF, 0xDF, 0xDF), 1));
        painter.setBrush(QColor(0xF5, 0xF5, 0xF5));
    }
    else
    {
        painter.setPen(QPen(QColor(0x37, 0x37, 0x37), 1));
        painter.setBrush(QColor(0x1C, 0x22, 0x32));
    }
    QRect foregroundRect(1, 1, width() - 2, height() - 2);
    painter.drawRoundedRect(foregroundRect, 6, 6);
    painter.restore();
}
