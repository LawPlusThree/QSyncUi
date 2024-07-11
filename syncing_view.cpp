#include "syncing_view.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include <QVBoxLayout>
#include"filecard.h"
#include"ElaScrollArea.h"
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
    _pushButton = new ElaPushButton("上传文件", this);
    _pushButton->setFixedSize(100, 40); // 设置按钮的固定大小
    _toggleButton = new ElaToggleButton("完成时提醒", this);
    _toggleButton->setFixedSize(100, 40); // 设置按钮的固定大小
    QWidget* pushButtonArea = new QWidget();
    pushButtonArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    pushButtonArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // 创建一个 ElaScrollPageArea 对象，用作切换按钮的容器，并指定其父对象
    //ElaScrollPageArea* pushButtonArea = new ElaScrollPageArea(this);
    // 为切换按钮容器设置水平布局
    QHBoxLayout* pushButtonLayout = new QHBoxLayout(pushButtonArea);
    pushButtonLayout->setContentsMargins(50, 0, 55, 0);
    // 将切换按钮控件添加到布局中
    pushButtonLayout->addWidget(_pushButton);
    pushButtonLayout->addWidget(_toggleButton);
    // 在布局中添加一个弹性空间，使得所有控件靠左对齐
    pushButtonLayout->addStretch();

    QWidget* catalogueArea = new QWidget();
    catalogueArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    catalogueArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    //ElaScrollPageArea* catalogueArea = new ElaScrollPageArea(this);
    QHBoxLayout* catalogueLayout = new QHBoxLayout(catalogueArea);
    catalogueLayout->setContentsMargins(18, 0, 25, 0);
    ElaText* catalogueText0 = new ElaText("", this);
    catalogueText0->setTextSize(16);
    catalogueText0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText0->setFixedSize(25,20);
    catalogueText0->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QHBoxLayout* catalogueArea0 = new QHBoxLayout();
    catalogueArea0->addWidget(catalogueText0, 0, Qt::AlignCenter);
    ElaText* catalogueText1 = new ElaText("文件名", this);
    catalogueText1->setTextSize(16);
    catalogueText1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QHBoxLayout* catalogueArea1 = new QHBoxLayout();
    catalogueArea1->addWidget(catalogueText1, 0, Qt::AlignLeft);
    ElaText* catalogueText2 = new ElaText("数据大小", this);
    catalogueText2->setTextSize(16);
    catalogueText2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText2->setFixedSize(100,20);
    catalogueText2->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QHBoxLayout* catalogueArea2 = new QHBoxLayout();
    catalogueArea2->addWidget(catalogueText2, 0, Qt::AlignCenter);
    ElaText* catalogueText4 = new ElaText("同步状态", this);
    catalogueText4->setTextSize(16);
    catalogueText4->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText4->setFixedSize(140,20);
    catalogueText4->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QHBoxLayout* catalogueArea4 = new QHBoxLayout();
    catalogueArea4->addWidget(catalogueText4, 0, Qt::AlignCenter);
    ElaText* catalogueText5 = new ElaText("操作", this);
    catalogueText5->setTextSize(16);
    catalogueText5->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText5->setFixedSize(60,20);
    catalogueText5->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QHBoxLayout* catalogueArea5 = new QHBoxLayout();
    catalogueArea5->addWidget(catalogueText5, 0, Qt::AlignCenter);
    catalogueLayout->addLayout(catalogueArea0);
    catalogueLayout->addLayout(catalogueArea1);
    catalogueLayout->addLayout(catalogueArea2);
    catalogueLayout->addLayout(catalogueArea4);
    catalogueLayout->addLayout(catalogueArea5);
    catalogueLayout->setStretchFactor(catalogueArea0, 25);
    catalogueLayout->setStretchFactor(catalogueArea1, 500);
    catalogueLayout->setStretchFactor(catalogueArea2, 100);
    catalogueLayout->setStretchFactor(catalogueArea4, 140);
    catalogueLayout->setStretchFactor(catalogueArea5, 60);


    ElaScrollArea* scrollArea = new ElaScrollArea();
    scrollArea->viewport()->setStyleSheet("background:transparent;");//设置背景透明

    _filecardProxy=new FileCardProxy(this);
    addFile("文件1",3,1.3,0,1);
    addFile("文件2",5,2,0,2);
    addFile("文件3fsgfhgsgskhdjfskjfbskhbfsbkhebvsbvhisbhjevkhba",3.5,3,52,3);
    addFile("文件vnjskbdkvsdjabjkvavshjajvnhsnvgdbdbsbsnjnsb",32,4,0,4);
    addFile("文件bb",4,51,5,5);
    addFile("文件6",256,65,10,6);
    addFile("文件7vbhksfbvbfshvbkjsbvfb",21,7,0,7);
    addFile("文件8",5,8,0,8);
    addFile("文件9vsnjkdfbvsvb",3,9,0,9);
    addFile("文件10v",3,10,100,10);

    scrollArea->setWidget(_filecardProxy);
    //scrollArea->setWidget(filesWidget); // 设置scrollArea的内容部件
    scrollArea->setWidgetResizable(true); // 允许scrollArea根据内容自动调整大小

    centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(pushButtonArea); // 将切换按钮容器添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addWidget(scrollArea);

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口中
}

SyncingPage::~SyncingPage()
{

}

void SyncingPage::addFile(QString filename, int datasize,int speed,int progress,int id)
{
    FileCard*newFile=new FileCard(filename,datasize,speed,progress,id);
    connect(newFile,&FileCard::Relieve,this,&SyncingPage::removeFile);
    _filecardProxy->addFileCard(newFile,id);
    totalProgress();
}

void SyncingPage::removeFile(int id)
{
    _filecardProxy->removeFileCard(id);
    totalProgress();
}

void SyncingPage::modifyFile(int d,int s,int p,int id)
{
    _filecardProxy->modify(d,s,p,id);
    totalProgress();
}

void SyncingPage::modifyFile(int p,int id)
{
    _filecardProxy->processing(p,id);
    totalProgress();
}

void SyncingPage::totalProgress()
{
    int totalpro=_filecardProxy->totalprogress();
    _progressBar->setValue(totalpro);
}
