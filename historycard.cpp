#include "historycard.h"

#include<QVBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QUrl>
#include"ElaCheckBox.h"
#include"ElaIcon.h"
#include"ElaIconButton.h"

HistoryCard::HistoryCard(QString f, QString d,QString t,bool u)
{
    filename=new ElaText(f);
    datasize=new ElaText(d);
    time=new ElaText(t);
    fullText=f;

    filename->setWordWrap(false);
    datasize->setWordWrap(false);
    time->setWordWrap(false);

    datasize->setAlignment(Qt::AlignCenter);
    time->setAlignment(Qt::AlignCenter);

    _checkBox = new ElaCheckBox("", this);
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
    filename->installEventFilter(this);
    filename->setCursor(Qt::PointingHandCursor); // 设置鼠标手势为手形，提示用户可以点击

    datasize->setTextSize(16);
    datasize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    datasize->setFixedSize(100,20);
    QVBoxLayout*dataSizeArea=new QVBoxLayout();
    dataSizeArea->addWidget(datasize,0,Qt::AlignCenter);

    time->setTextSize(16);
    time->setFixedWidth(160);
    QVBoxLayout*timeArea=new QVBoxLayout();
    ElaText*text;
    if(u)
        text=new ElaText("上传于");
    else
        text=new ElaText("下载于");
    text->setTextSize(16);
    text->setWordWrap(false);
    timeArea->addWidget(text,0,Qt::AlignCenter);
    timeArea->addWidget(time,0,Qt::AlignCenter);

    ElaIconButton* checkIcon=new ElaIconButton(ElaIconType::Check,30);
    checkIcon->setFixedSize(30,30);
    checkIcon->setEnabled(false); // 禁用按钮
    QVBoxLayout*bingoArea=new QVBoxLayout();
    bingoArea->addWidget(checkIcon,0,Qt::AlignCenter);

    QHBoxLayout*HistoryCardArea=new QHBoxLayout(this);
    HistoryCardArea->addLayout(checkBoxArea);
    HistoryCardArea->addWidget(filenameWidget);
    HistoryCardArea->addLayout(dataSizeArea);
    HistoryCardArea->addLayout(timeArea);
    HistoryCardArea->addLayout(bingoArea);
    HistoryCardArea->setStretchFactor(checkBoxArea,25);
    HistoryCardArea->setStretchFactor(filenameWidget,500);
    HistoryCardArea->setStretchFactor(dataSizeArea,100);
    HistoryCardArea->setStretchFactor(timeArea,140);
    HistoryCardArea->setStretchFactor(bingoArea,60);

    filenameWidget->setStyleSheet("QToolTip { color: #5C5C5C; background-color: #F9F9F9; border: 1px solid #808080; border-radius: 3px; }");
}

bool HistoryCard::eventFilter(QObject *watched, QEvent *event) {
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
