#include "DirCard.h"

#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QUrl>

#include"ElaCheckBox.h"
#include "private/DirCardPrivate.h"
#include"ElaPushButton.h"
#include"ElaIconButton.h"

DirCard::DirCard(QString f, QString c,quint64 d,QString b,int syncStatus,int Id)
{
    filename=new ElaText(f);
    fullText = f;
    cloudname=new ElaText(c);
    cfullText=c;

    if(syncStatus==1)//双向
    {
        syncBtn=new ElaIconButton(ElaIconType::ArrowRightArrowLeft,20);
        syncBtn->setEnabled(true);
    }
    else if(syncStatus==2)//上传
    {
        syncBtn=new ElaIconButton(ElaIconType::ArrowRight,20);
        syncBtn->setEnabled(true);
    }
    else if(syncStatus==3)//下载
    {
        syncBtn=new ElaIconButton(ElaIconType::ArrowLeft,20);
        syncBtn->setEnabled(true);
    }
    else//禁用
    {
        syncBtn=new ElaIconButton(ElaIconType::ArrowRightArrowLeft,20);
        syncBtn->setEnabled(false);
    }
    syncBtn->setFixedSize(30, 30);
    QHBoxLayout*syncArea=new QHBoxLayout();
    syncArea->addWidget(syncBtn);
    syncArea->setAlignment(Qt::AlignCenter);

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
    datasize = new ElaText(size, this);
    bindtime=new ElaText(b);
    id=Id;

    filename->setWordWrap(false);//禁止换行
    cloudname->setWordWrap(false);
    datasize->setWordWrap(false);
    bindtime->setWordWrap(false);
    //progress->setWordWrap(false);

    datasize->setAlignment(Qt::AlignCenter);
    bindtime->setAlignment(Qt::AlignCenter);
    //progress->setAlignment(Qt::AlignCenter);

    _checkBox = new ElaCheckBox("", this);
    _checkBox->setFixedSize(25, 25);
    QVBoxLayout* checkBoxArea = new QVBoxLayout();
    checkBoxArea->addWidget(_checkBox, 0, Qt::AlignCenter); // 将_checkBox放在布局的最左侧

    filename->setTextSize(16);
    filename->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QWidget* filenameWidget = new QWidget();
    filenameWidget->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    filenameWidget->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    filenameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* filenameArea = new QVBoxLayout(filenameWidget);
    filenameArea->addWidget(filename, 0, Qt::AlignLeft); // 将文件名放在左侧
    filename->installEventFilter(this);
    filename->setCursor(Qt::PointingHandCursor); // 设置鼠标手势为手形，提示用户可以点击

    cloudname->setTextSize(16);
    cloudname->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QWidget* cloudnameWidget = new QWidget();
    cloudnameWidget->setWindowFlags(Qt::FramelessWindowHint);
    cloudnameWidget->setAttribute(Qt::WA_TranslucentBackground);
    cloudnameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* cloudnameArea = new QVBoxLayout(cloudnameWidget);
    cloudnameArea->addWidget(cloudname, 0, Qt::AlignLeft);

    datasize->setTextSize(16);
    datasize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    datasize->setFixedSize(100,20);
    QVBoxLayout* dataSizeArea = new QVBoxLayout();
    dataSizeArea->addWidget(datasize, 0, Qt::AlignCenter);

    bindtime->setTextSize(16);
    bindtime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bindtime->setFixedSize(140,20);
    QVBoxLayout* bindTimeArea = new QVBoxLayout();
    bindTimeArea->addWidget(bindtime, 0, Qt::AlignCenter);

    //QVBoxLayout*pauseArea=new QVBoxLayout();
    //pauseBtn=new ElaToggleButton("暂时停止");
    //pauseBtn->setFixedSize(80,30);
    //pauseArea->addWidget(pauseBtn,0,Qt::AlignCenter);

    //QVBoxLayout*relieveArea=new QVBoxLayout();
    //relieveBtn=new ElaPushButton("解除绑定");
    //relieveBtn->setFixedSize(80,30);
    //relieveArea->addWidget(relieveBtn,0,Qt::AlignCenter);

    pauseBtn = new ElaIconButton(ElaIconType::Pause, 20);
    relieveBtn = new ElaIconButton(ElaIconType::LinkSlash, 20);
    pauseBtn->setFixedSize(30, 30);
    relieveBtn->setFixedSize(30, 30);

    QHBoxLayout* actionArea = new QHBoxLayout();
    actionArea->addWidget(pauseBtn);
    actionArea->addWidget(relieveBtn);
    actionArea->setAlignment(Qt::AlignCenter);

    QHBoxLayout* FileCardArea = new QHBoxLayout(this);
    FileCardArea->addLayout(checkBoxArea); // 将_checkBox布局添加到卡片布局中
    FileCardArea->addWidget(filenameWidget); // 将文件名布局添加到卡片布局中，紧跟_checkBox
    FileCardArea->addLayout(syncArea);
    FileCardArea->addWidget(cloudnameWidget);
    FileCardArea->addLayout(dataSizeArea);
    FileCardArea->addLayout(bindTimeArea);
    FileCardArea->addLayout(actionArea);
    FileCardArea->setStretchFactor(checkBoxArea, 25);
    FileCardArea->setStretchFactor(filenameWidget, 235);
    FileCardArea->setStretchFactor(syncArea, 30);
    FileCardArea->setStretchFactor(cloudnameWidget, 235);
    FileCardArea->setStretchFactor(dataSizeArea, 100);
    FileCardArea->setStretchFactor(bindTimeArea, 140);
    FileCardArea->setStretchFactor(actionArea, 60);

    connect(relieveBtn,&ElaPushButton::clicked,this,&DirCard::on_relieveBtn_clicked);
    connect(pauseBtn,&ElaIconButton::clicked,this,&DirCard::on_pauseBtn_clicked);
    cloudnameWidget->setStyleSheet("QToolTip { color: #5C5C5C; background-color: #F9F9F9; border: 1px solid #808080; border-radius: 3px; }");
    filenameWidget->setStyleSheet("QToolTip { color: #5C5C5C; background-color: #F9F9F9; border: 1px solid #808080; border-radius: 3px; }");
}

ElaCheckBox* DirCard::getCheckBox() const
{
    return _checkBox;
}

void DirCard::on_relieveBtn_clicked()
{
    qDebug()<<"发出前的id"<<id;
    emit relieve(id);
}

void DirCard::on_pauseBtn_clicked()
{
    if(pauseBtn->getAwesome()==ElaIconType::Pause)
        pauseBtn->setAwesome(ElaIconType::Play);
    else
        pauseBtn->setAwesome(ElaIconType::Pause);
}

bool DirCard::ischecked()
{
    return _checkBox->isChecked();
}

void DirCard::modify(quint64 d,QString b)
{
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
    datasize->setText(size);
    bindtime->setText(b);
}

bool DirCard::eventFilter(QObject *watched, QEvent *event) {
    if (watched == filename && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QString filePath = this->fullText;
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
            return true; // 事件处理完毕，不再传递
        }
    }
    return QObject::eventFilter(watched, event); // 对于其他事件，保持默认处理
}
