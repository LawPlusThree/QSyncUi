#include"historyview.h"
#include<QVBoxLayout>
#include"ElaText.h"
#include<QHBoxLayout>
#include"ElaWidget.h"
#include"ElaText.h"
#include"ElaPushButton.h"
#include"ElaToggleButton.h"
#include"ElaScrollArea.h"

#include"historyviewcard.h"

HistoryViewPage::HistoryViewPage(QWidget* parent): ElaScrollPage(parent)
{
    QWidget* centralWidget = new QWidget(this); // 创建中心部件
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(0, 0, 5, 0); // 设置布局边距,5为到右边的距离
    centralWidget->setWindowTitle("查看历史"); // 设置窗口标题

    // 创建一个 ElaProgressBar 对象，用作进度条
    _progressBar = new ElaProgressBar(this);
    // 设置QProgressBar的大小策略为水平方向可扩展，垂直方向固定
    _progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 设置最小尺寸，可以适当调整宽度以减少填充效果
    _progressBar->setMinimumSize(100, 20); // 调整最小宽度为100，最小高度20
    // 设置进度条的最小值和最大值
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0); // 表示不确定的进度
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

    QWidget* catalogueArea = new QWidget();
    catalogueArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    catalogueArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    //ElaScrollPageArea* catalogueArea = new ElaScrollPageArea(this);
    QHBoxLayout* catalogueLayout = new QHBoxLayout(catalogueArea);
    catalogueLayout->setContentsMargins(10, 0, 15, 0);
    ElaText* catalogueText1 = new ElaText("版本名", this);
    catalogueText1->setTextSize(15);
    catalogueText1->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText2 = new ElaText("数据大小", this);
    catalogueText2->setTextSize(15);
    catalogueText2->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText3 = new ElaText("同步时间", this);
    catalogueText3->setTextSize(15);
    catalogueText3->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText4 = new ElaText("操作", this);
    catalogueText4->setTextSize(15);
    catalogueText4->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    catalogueLayout->addWidget(catalogueText1);
    catalogueLayout->addWidget(catalogueText2);
    catalogueLayout->addWidget(catalogueText3);
    catalogueLayout->addWidget(catalogueText4);

    ElaScrollArea* scrollArea = new ElaScrollArea();
    scrollArea->viewport()->setStyleSheet("background:transparent;");//设置背景透明

    HistoryViewCard *HistoryViewCard1 = new HistoryViewCard("历史版本1","3.5G","2024.7.2");
    HistoryViewCard *HistoryViewCard2 = new HistoryViewCard("历史版本2","3.5G","2024.5.3");
    HistoryViewCard *HistoryViewCard3 = new HistoryViewCard("历史版本3","3.5G","2024.12.6");
    HistoryViewCard *HistoryViewCard4 = new HistoryViewCard("历史版本4","3.5G","2024.1.6");
    HistoryViewCard *HistoryViewCard5 = new HistoryViewCard("历史版本5","3.5G","2024.1.26");
    HistoryViewCard *HistoryViewCard6 = new HistoryViewCard("历史版本6","3.5G","2024.7.9");
    HistoryViewCard *HistoryViewCard7 = new HistoryViewCard("历史版本7","3.5G","2024.7.17");
    HistoryViewCard *HistoryViewCard8 = new HistoryViewCard("历史版本8","3.5G","2024.8.8");
    HistoryViewCard *HistoryViewCard9 = new HistoryViewCard("历史版本9","3.5G","2024.6.9");
    HistoryViewCard *HistoryViewCard10 = new HistoryViewCard("历史版本10","3.5G","2024.10.10");
    HistoryViewCard *HistoryViewCard11 = new HistoryViewCard("历史版本11","45.14G","1919.8.10");
    QWidget* filesWidget=new QWidget();
    filesLayout=new QVBoxLayout(filesWidget);
    filesLayout->addWidget(HistoryViewCard1);
    filesLayout->addWidget(HistoryViewCard2);
    filesLayout->addWidget(HistoryViewCard3);
    filesLayout->addWidget(HistoryViewCard4);
    filesLayout->addWidget(HistoryViewCard5);
    filesLayout->addWidget(HistoryViewCard6);
    filesLayout->addWidget(HistoryViewCard7);
    filesLayout->addWidget(HistoryViewCard8);
    filesLayout->addWidget(HistoryViewCard9);
    filesLayout->addWidget(HistoryViewCard10);
    filesLayout->addWidget(HistoryViewCard11);

    scrollArea->setWidget(filesWidget); // 设置scrollArea的内容部件
    scrollArea->setWidgetResizable(true); // 允许scrollArea根据内容自动调整大小

    centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addWidget(scrollArea); // 将scrollArea添加到布局中

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口
}

void HistoryViewPage::addHistoryViewCard(QString filename,QString datasize,QString bingtime)
{
    HistoryViewCard*newHV=new HistoryViewCard(filename,datasize,bingtime);
    filesLayout->addWidget(newHV);
}
