#include "historysync_view.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include <QVBoxLayout>
#include"historycard.h"
#include"ElaScrollArea.h"
#include"historycardproxy.h"

HistorysyncPage::HistorysyncPage(QWidget* parent)
    : ElaScrollPage(parent)
{
    QWidget* centralWidget = new QWidget(this); // 创建中心部件
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(0, 0, 5, 0); // 设置布局边距,5为到右边的距离
    centralWidget->setWindowTitle("历史同步"); // 设置窗口标题

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
    ElaText* catalogueText4 = new ElaText("同步时间", this);
    catalogueText4->setTextSize(16);
    catalogueText4->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText4->setFixedSize(140,20);
    catalogueText4->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea4 = new QVBoxLayout();
    catalogueArea4->addWidget(catalogueText4, 0, Qt::AlignCenter);
    ElaText* catalogueText5 = new ElaText("状态", this);
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

    _historycardPage=new HistoryCardProxy(this);
    /*addHistory("文件1554524527252572752","3.5GB","2024.7.1",0);
    addHistory("文件hadgiauyfgugfauwygfuygfuewedfcyufg","3.5GB","2024.7.2",0);
    addHistory("文件3","3.5GB","2024.7.3",1);
    addHistory("文件4","3.5GB","2024.7.4",0);
    addHistory("文件5","3.5GB","2024.7.5",1);
    addHistory("文件6","3.5GB","2024.7.6",1);
    addHistory("文件7","3.5GB","2024.7.7",1);
    addHistory("文件8","3.5GB","2024.7.8",0);
    addHistory("文件9","3.5GB","2024.7.9",0);
    addHistory("文件10","3.5GB","2024.7.10",1);*/

    scrollArea->setWidget(_historycardPage);
    scrollArea->setWidgetResizable(true); // 允许scrollArea根据内容自动调整大小

    centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addWidget(scrollArea);

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口中
}

HistorysyncPage::~HistorysyncPage()
{
}

void HistorysyncPage::addHistory(QString filename, QString datasize,QString time,bool upif)
{
    /*HistoryCard*historyCard=new HistoryCard(filename,datasize,time,upif);
    filesLayout->addWidget(historyCard);*/
    HistoryCard*newHistory=new HistoryCard(filename,datasize,time,upif);
    _historycardPage->addHistoryCard(newHistory);
    QFontMetrics metrics(newHistory->filename->font());
    QString elidedText = metrics.elidedText(newHistory->fullText, Qt::ElideMiddle, filenameWidget->width()-20);
    newHistory->filename->setText(elidedText);
    newHistory->filename->setToolTip(newHistory->fullText);
}

void HistorysyncPage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    auto thisMap=this->_historycardPage->cardVector;
    for (auto &x:thisMap){
        QFontMetrics metrics(x->filename->font());
        QString elidedText = metrics.elidedText(x->fullText, Qt::ElideMiddle, filenameWidget->width()-20);
        x->filename->setText(elidedText);
        x->filename->setToolTip(x->fullText);
    }
}

void HistorysyncPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    auto thisMap=this->_historycardPage->cardVector;
    for (auto &x:thisMap){
        QFontMetrics metrics(x->filename->font());
        QString elidedText = metrics.elidedText(x->fullText, Qt::ElideMiddle, filenameWidget->width()-20);
        x->filename->setText(elidedText);
        x->filename->setToolTip(x->fullText);
    }
}
