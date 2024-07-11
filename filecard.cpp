#include "filecard.h"

#include<QVBoxLayout>
#include"ElaCheckBox.h"
#include"ElaIconButton.h"
#include"ElaProgressBar.h"
FileCard::FileCard(QString f, QString d,QString s,QString p)
{
    // 创建ElaText对象来显示文件名，而不是直接在_checkBox中显示
    filename = new ElaText(f, this);
    datasize = new ElaText(d, this);
    speed = new ElaText(s, this);
    progress = new ElaText(p, this);
    fullText = f;

    filename->setWordWrap(false);//禁止换行
    datasize->setWordWrap(false);
    speed->setWordWrap(false);
    progress->setWordWrap(false);

    datasize->setAlignment(Qt::AlignCenter);
    speed->setAlignment(Qt::AlignCenter);
    progress->setAlignment(Qt::AlignCenter);

    // 创建一个没有文字的_checkBox
    _checkBox = new ElaCheckBox("", this);
    _checkBox->setFixedSize(25, 25);
    QVBoxLayout* checkBoxArea = new QVBoxLayout();
    checkBoxArea->addWidget(_checkBox, 0, Qt::AlignCenter); // 将_checkBox放在布局的最左侧

    // 设置文件名文字的布局
    filename->setTextSize(16);
    filename->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QWidget* filenameWidget = new QWidget();
    filenameWidget->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    filenameWidget->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    filenameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* filenameArea = new QVBoxLayout(filenameWidget);
    filenameArea->addWidget(filename, 0, Qt::AlignLeft); // 将文件名放在左侧

    datasize->setTextSize(16);
    datasize->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    datasize->setFixedSize(100,20);
    QVBoxLayout* dataSizeArea = new QVBoxLayout();
    dataSizeArea->addWidget(datasize, 0, Qt::AlignCenter);
    //dataSizeArea->SetFixedSize(50, 20);

    proBar = new ElaProgressBar(this);
    proBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    proBar->setFixedSize(140, 20);
    proBar->setMinimum(0);
    proBar->setMaximum(0);
    progress->setTextSize(16);
    speed->setTextSize(16);
    QHBoxLayout* upBarArea = new QHBoxLayout();
    upBarArea->addWidget(speed,0,Qt::AlignCenter);
    upBarArea->addWidget(progress,0,Qt::AlignCenter);
    QVBoxLayout* proBarArea = new QVBoxLayout();
    proBarArea->addLayout(upBarArea);
    proBarArea->addWidget(proBar, 0, Qt::AlignCenter);
    proBarArea->setAlignment(Qt::AlignCenter);

    pauseBtn = new ElaIconButton(ElaIconType::CirclePause, 20);
    relieveBtn = new ElaIconButton(ElaIconType::CircleXmark, 20);
    pauseBtn->setFixedSize(30, 30);
    relieveBtn->setFixedSize(30, 30);

    QHBoxLayout* actionArea = new QHBoxLayout();
    actionArea->addWidget(pauseBtn);
    actionArea->addWidget(relieveBtn);
    actionArea->setAlignment(Qt::AlignCenter);

    QHBoxLayout* FileCardArea = new QHBoxLayout(this);
    FileCardArea->addLayout(checkBoxArea); // 将_checkBox布局添加到卡片布局中
    FileCardArea->addWidget(filenameWidget); // 将文件名布局添加到卡片布局中，紧跟_checkBox
    FileCardArea->addLayout(dataSizeArea);
    FileCardArea->addLayout(proBarArea);
    FileCardArea->addLayout(actionArea);
    FileCardArea->setStretchFactor(checkBoxArea, 25);
    FileCardArea->setStretchFactor(filenameWidget, 500);
    FileCardArea->setStretchFactor(dataSizeArea, 100);
    FileCardArea->setStretchFactor(proBarArea, 140);
    FileCardArea->setStretchFactor(actionArea, 60);

    connect(relieveBtn, &ElaIconButton::clicked, this, &FileCard::on_relieveBtn_clicked);
    connect(pauseBtn, &ElaIconButton::clicked, this, &FileCard::on_pauseBtn_clicked);

}

void FileCard::on_relieveBtn_clicked()
{
    emit Relieve();
}

void FileCard::on_pauseBtn_clicked()
{
    if(pauseBtn->getAwesome()==ElaIconType::CirclePause)
        pauseBtn->setAwesome(ElaIconType::CirclePlay);
    else
        pauseBtn->setAwesome(ElaIconType::CirclePause);
}
