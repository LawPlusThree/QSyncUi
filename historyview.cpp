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
#include"historyviewcardproxy.h"

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
    catalogueLayout->setContentsMargins(18, 0, 20, 0);
    ElaText* catalogueText0 = new ElaText("", this);
    catalogueText0->setTextSize(16);
    catalogueText0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText0->setFixedSize(25,20);
    catalogueText0->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea0 = new QVBoxLayout();
    catalogueArea0->addWidget(catalogueText0, 0, Qt::AlignCenter);

    ElaText* catalogueText1 = new ElaText("本地文件夹", this);
    catalogueText1->setTextSize(16);
    catalogueText1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //QWidget* filenameWidget = new QWidget();
    filenameWidget->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    filenameWidget->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    filenameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* filenameArea = new QVBoxLayout(filenameWidget);
    filenameArea->addWidget(catalogueText1, 0, Qt::AlignLeft);

    ElaText* catalogueText2 = new ElaText("云端文件夹", this);
    catalogueText2->setTextSize(16);
    catalogueText2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    cloudnameWidget->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    cloudnameWidget->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    cloudnameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* cloudnameArea = new QVBoxLayout(cloudnameWidget);
    cloudnameArea->addWidget(catalogueText2, 0, Qt::AlignLeft);

    catalogueLayout->addLayout(catalogueArea0);
    catalogueLayout->addWidget(filenameWidget);
    catalogueLayout->addWidget(cloudnameWidget);
    catalogueLayout->setStretchFactor(catalogueArea0,25);
    catalogueLayout->setStretchFactor(filenameWidget,430);
    catalogueLayout->setStretchFactor(cloudnameWidget,430);

    ElaScrollArea* scrollArea = new ElaScrollArea();
    scrollArea->viewport()->setStyleSheet("background:transparent;");//设置背景透明

    _historyviewcardPage=new HistoryviewCardProxy(this);
    addHistoryViewCard("C:/Users/27284/1111111111111Documents/114514","/mk111111111555757575775757711111111111szyjbyl");
    addHistoryViewCard("C:/Users/27284/Documents/books","/books");
    addSubCard("C:/Users/27284/Documents/books","4.6",43000,"2024.7.15 09:28:20");
    addSubCard("C:/Users/27284/Documents/books","4.7",555007,"2024.7.16 10:30:25");
    addSubCard("C:/Users/27284/1111111111111Documents/114514","4.7",555007,"2024.7.16 15:42:16");
    addHistoryViewCard("C:/Users/114514/Documents/books","/1919810");

    scrollArea->setWidget(_historyviewcardPage);
    scrollArea->setWidgetResizable(true); // 允许scrollArea根据内容自动调整大小

    centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addWidget(scrollArea); // 将scrollArea添加到布局中

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口
}

void HistoryViewPage::addSubCard(QString filename,QString versionID,quint64 datasize,QString bindtime)
{
    _historyviewcardPage->addSubCard(filename,versionID,datasize,bindtime);
}

void HistoryViewPage::addHistoryViewCard(QString filename,QString cloudname)
{
    HistoryViewCard*newHV=new HistoryViewCard(filename,cloudname);
    _historyviewcardPage->addHistoryviewCard(newHV);

    QFontMetrics metrics(newHV->filename->font());
    QString elidedText = metrics.elidedText(newHV->fullText, Qt::ElideMiddle, filenameWidget->width()-30);
    newHV->filename->setText(elidedText);
    newHV->filename->setToolTip(newHV->fullText);
    QFontMetrics metrics2(newHV->cloudname->font());
    QString elidedText2 = metrics2.elidedText(newHV->cfullText, Qt::ElideMiddle, cloudnameWidget->width()-30);
    newHV->cloudname->setText(elidedText2);
    newHV->cloudname->setToolTip(newHV->cfullText);
}

void HistoryViewPage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    auto thisMap=this->_historyviewcardPage->cardMap;
    for (auto &x:thisMap){
        QFontMetrics metrics(x->filename->font());
        QString elidedText = metrics.elidedText(x->fullText, Qt::ElideMiddle, filenameWidget->width()-30);
        x->filename->setText(elidedText);
        x->filename->setToolTip(x->fullText);

        QFontMetrics metrics2(x->cloudname->font());
        QString elidedText2 = metrics2.elidedText(x->cfullText, Qt::ElideMiddle, cloudnameWidget->width()-30);
        x->cloudname->setText(elidedText2);
        x->cloudname->setToolTip(x->cfullText);
    }
}

void HistoryViewPage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    auto thisMap=this->_historyviewcardPage->cardMap;
    for (auto &x:thisMap){
        QFontMetrics metrics(x->filename->font());
        QString elidedText = metrics.elidedText(x->fullText, Qt::ElideMiddle, filenameWidget->width()-30);
        x->filename->setText(elidedText);
        x->filename->setToolTip(x->fullText);

        QFontMetrics metrics2(x->cloudname->font());
        QString elidedText2 = metrics2.elidedText(x->cfullText, Qt::ElideMiddle, cloudnameWidget->width()-30);
        x->cloudname->setText(elidedText2);
        x->cloudname->setToolTip(x->cfullText);
    }
}
