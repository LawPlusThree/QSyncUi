#include "syncing_view.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include"ElaCheckBox.h"
#include"ElaIconButton.h"
#include <QVBoxLayout>
#include"filecard.h"
#include"ElaScrollArea.h"
#include <QDebug>
#include <QApplication>
#include <QTimer>
#include <QObject>
#include"filecardproxy.h"

SyncingPage::SyncingPage(QWidget* parent)
    : ElaScrollPage(parent)
{
    QWidget* centralWidget = new QWidget(this); // 创建中心部件
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(0, 0, 5, 0); // 设置布局边距,5为到右边的距离
    centralWidget->setWindowTitle("正在同步"); // 设置窗口标题

    // 创建一个 ElaProgressBar 对象，用作进度条
    _progressBar = new ElaProgressBar(this);
    // 设置QProgressBar的大小策略为水平方向可扩展，垂直方向固定
    _progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 设置最小尺寸，可以适当调整宽度以减少填充效果
    _progressBar->setMinimumSize(100, 20); // 调整最小宽度为100，最小高度20
    // 设置进度条的最小值和最大值
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(100); // 表示不确定的进度
    _progressBar->setValue(0);
    QWidget* progressBarArea = new QWidget();
    progressBarArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    progressBarArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // 创建一个 ElaScrollPageArea 对象，用作进度条的容器
    //ElaScrollPageArea* progressBarArea = new ElaScrollPageArea(this);
    // 为进度条容器设置水平布局
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    progressBarLayout->setContentsMargins(50, 0, 55, 0); // 设置布局的边距
    // 创建一个 ElaText 对象，用于显示进度条的文本标签
    ElaText* progressBarText = new ElaText("总进度：", this);
    // 设置进度条文本标签的字体大小为 15
    progressBarText->setTextSize(15);
    // 将进度条文本标签和进度条控件添加到布局中
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);

    // 创建一个 ElaToggleButton 对象，设置其标签为 "Head" 并指定其父对象
    /*_pushButton = new ElaPushButton("上传文件", this);
    _pushButton->setFixedSize(100, 40); // 设置按钮的固定大小
    _toggleButton = new ElaToggleButton("完成时提醒", this);
    _toggleButton->setFixedSize(100, 40); // 设置按钮的固定大小*/
    _PauseButton = new ElaPushButton("暂停同步",this);
    _PauseButton->setFixedSize(100, 40);
    _CancelButton = new ElaPushButton("停止同步",this);
    _CancelButton->setFixedSize(100, 40);
    _selectAllButton = new ElaPushButton("全选/全不选", this);
    _selectAllButton->setFixedSize(100, 40);
    QWidget* pushButtonArea = new QWidget();
    pushButtonArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    pushButtonArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // 创建一个 ElaScrollPageArea 对象，用作切换按钮的容器，并指定其父对象
    //ElaScrollPageArea* pushButtonArea = new ElaScrollPageArea(this);
    // 为切换按钮容器设置水平布局
    QHBoxLayout* pushButtonLayout = new QHBoxLayout(pushButtonArea);
    pushButtonLayout->setContentsMargins(50, 0, 55, 0);
    // 将切换按钮控件添加到布局中
    //pushButtonLayout->addWidget(_pushButton);
    //pushButtonLayout->addWidget(_toggleButton);
    // 在布局中添加一个弹性空间，使得所有控件靠左对齐
    pushButtonLayout->addStretch();

    pushButtonLayout->addWidget(_PauseButton);
    pushButtonLayout->addWidget(_CancelButton);
    pushButtonLayout->addWidget(_selectAllButton);
    connect(_selectAllButton, &QPushButton::clicked, this, &SyncingPage::toggleSelectAll);    connect(_PauseButton,&ElaPushButton::clicked,[=](){
        PauseChecked();
    });
    connect(_CancelButton,&ElaPushButton::clicked,[=](){
        CancelChecked();
    });
    _PauseButton->hide();
    _CancelButton->hide();

    QWidget* catalogueArea = new QWidget();
    catalogueArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    catalogueArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    //ElaScrollPageArea* catalogueArea = new ElaScrollPageArea(this);
    QHBoxLayout* catalogueLayout = new QHBoxLayout(catalogueArea);
    catalogueLayout->setContentsMargins(18, 0, 20, 0);
    ElaText* catalogueText0 = new ElaText("", this);
    catalogueText0->setTextSize(16);
    catalogueText0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText0->setFixedSize(25,20);
    catalogueText0->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea0 = new QVBoxLayout();
    catalogueArea0->addWidget(catalogueText0, 0, Qt::AlignCenter);

    ElaText* catalogueText1 = new ElaText("文件名", this);
    catalogueText1->setTextSize(16);
    catalogueText1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //QWidget* filenameWidget = new QWidget();
    filenameWidget->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    filenameWidget->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    filenameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* filenameArea = new QVBoxLayout(filenameWidget);
    filenameArea->addWidget(catalogueText1, 0, Qt::AlignLeft);

    ElaText* catalogueText2 = new ElaText("数据大小", this);
    catalogueText2->setTextSize(16);
    catalogueText2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText2->setFixedSize(100,20);
    catalogueText2->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea2 = new QVBoxLayout();
    catalogueArea2->addWidget(catalogueText2, 0, Qt::AlignCenter);
    ElaText* catalogueText4 = new ElaText("同步状态", this);
    catalogueText4->setTextSize(16);
    catalogueText4->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText4->setFixedSize(140,20);
    catalogueText4->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea4 = new QVBoxLayout();
    catalogueArea4->addWidget(catalogueText4, 0, Qt::AlignCenter);
    ElaText* catalogueText5 = new ElaText("操作", this);
    catalogueText5->setTextSize(16);
    catalogueText5->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText5->setFixedSize(60,20);
    catalogueText5->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QHBoxLayout* catalogueArea5 = new QHBoxLayout();
    catalogueArea5->addWidget(catalogueText5, 0, Qt::AlignCenter);
    catalogueLayout->addLayout(catalogueArea0);
    catalogueLayout->addWidget(filenameWidget);
    catalogueLayout->addLayout(catalogueArea2);
    catalogueLayout->addLayout(catalogueArea4);
    catalogueLayout->addLayout(catalogueArea5);
    catalogueLayout->setStretchFactor(catalogueArea0, 25);
    catalogueLayout->setStretchFactor(filenameWidget, 500);
    catalogueLayout->setStretchFactor(catalogueArea2, 100);
    catalogueLayout->setStretchFactor(catalogueArea4, 140);
    catalogueLayout->setStretchFactor(catalogueArea5, 60);


    ElaScrollArea* scrollArea = new ElaScrollArea();
    scrollArea->viewport()->setStyleSheet("background:transparent;");//设置背景透明

    _filecardProxy=new FileCardProxy(this);

    scrollArea->setWidget(_filecardProxy);
    //scrollArea->setWidget(filesWidget); // 设置scrollArea的内容部件
    scrollArea->setWidgetResizable(true); // 允许scrollArea根据内容自动调整大小

    connect(_filecardProxy, &FileCardProxy::checkBoxToggled, this, &SyncingPage::buttonShowHide);
    connect(_filecardProxy, &FileCardProxy::removeCard, this, &SyncingPage::buttonShowHide);

    centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(pushButtonArea); // 将切换按钮容器添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addWidget(scrollArea);

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口中
}

SyncingPage::~SyncingPage()
{

}

void SyncingPage::addFile(QString filename, quint64 datasize,double speed,int progress,int syncStatus,int id)
{
    FileCard*newFile=new FileCard(filename,datasize,speed,progress,syncStatus,id);
    connect(newFile,&FileCard::Relieve,this,&SyncingPage::removeFile);
    _filecardProxy->addFileCard(newFile,id);
    totalProgress();
    QFontMetrics metrics(newFile->filename->font());
    QString elidedText = metrics.elidedText(newFile->fullText, Qt::ElideMiddle, filenameWidget->width()-20);
    newFile->filename->setText(elidedText);
    newFile->filename->setToolTip(newFile->fullText);
}

void SyncingPage::removeFile(int id)
{
    _filecardProxy->removeFileCard(id);
    totalProgress();
}


void SyncingPage::modifyFile(quint64 totalSize,quint64 currentSize,int id)
{
    _filecardProxy->modify(totalSize,currentSize,id);
    totalProgress();
}

void SyncingPage::totalProgress()
{
    int totalpro=_filecardProxy->totalprogress();
    _progressBar->setValue(totalpro);
}

void SyncingPage::PauseChecked()
{

    QMapIterator<int, FileCard*> i(_filecardProxy->cardMap);
    while (i.hasNext())
    {
        i.next();
        FileCard *card = i.value();
        if(card->ischecked())
            // 检查当前按钮状态，如果是播放状态，则切换到暂停状态
            if(card->pauseBtn->getAwesome() == ElaIconType::CirclePlay)
            {
                card->pauseBtn->setAwesome(ElaIconType::CirclePause);
            }
        // 触发暂停按钮的点击事件，以调用on_pauseBtn_clicked槽函数
        emit card->pauseBtn->clicked();
        }
}

void SyncingPage::CancelChecked()
{
    QMapIterator<int, FileCard*> i(_filecardProxy->cardMap);
    while (i.hasNext())
    {
        i.next();
        int id = i.key();
        FileCard *card = i.value();
        if(card->ischecked())
            removeFile(id);
        }
}

// 重写resizeEvent
void SyncingPage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    auto thisMap=this->_filecardProxy->cardMap;
    for (auto &x:thisMap){
        QFontMetrics metrics(x->filename->font());
        QString elidedText = metrics.elidedText(x->fullText, Qt::ElideMiddle, filenameWidget->width()-20);
        x->filename->setText(elidedText);
        x->filename->setToolTip(x->fullText);
    }
}

void SyncingPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    auto thisMap=this->_filecardProxy->cardMap;
    for (auto &x:thisMap){
        QFontMetrics metrics(x->filename->font());
        QString elidedText = metrics.elidedText(x->fullText, Qt::ElideMiddle, filenameWidget->width()-20);
        x->filename->setText(elidedText);
        x->filename->setToolTip(x->fullText);
    }
}

void SyncingPage::toggleSelectAll()
{
    // 切换所有文件卡片的多选框状态
    QMapIterator<int, FileCard*> i(_filecardProxy->cardMap);
    while (i.hasNext()) {
        i.next();
        FileCard *card = i.value();
        card->_checkBox->setChecked(!card->_checkBox->isChecked());
    }

    // 切换按钮状态
    _selectAllButton->setChecked(!_selectAllButton->isChecked());
}

void SyncingPage::buttonShowHide()
{
    QMapIterator<int, FileCard*> i(_filecardProxy->cardMap);
    bool allNotChecked = true;
    while (i.hasNext())
    {
        i.next();
        FileCard *card = i.value();
        if(card->ischecked())
        {
            allNotChecked = false;
            break;
        }
    }
    if(!allNotChecked) {
        _PauseButton->show(); // 如果复选框被勾选，显示按钮
        _CancelButton->show();
    } else {
        _PauseButton->hide();
        _CancelButton->hide(); // 如果复选框未被勾选，隐藏按钮
    }
}
