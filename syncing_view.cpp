#include "syncing_view.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaToggleButton.h"
#include <QVBoxLayout>

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
    catalogueLayout->setContentsMargins(10, 0, 15, 0);
    ElaText* catalogueText1 = new ElaText("文件名", this);
    catalogueText1->setTextSize(15);
    catalogueText1->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText2 = new ElaText("数据大小", this);
    catalogueText2->setTextSize(15);
    catalogueText2->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText3 = new ElaText("网络速度", this);
    catalogueText3->setTextSize(15);
    catalogueText3->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText4 = new ElaText("同步状态", this);
    catalogueText4->setTextSize(15);
    catalogueText4->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    ElaText* catalogueText5 = new ElaText("操作", this);
    catalogueText5->setTextSize(15);
    catalogueText5->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    catalogueLayout->addWidget(catalogueText1);
    catalogueLayout->addWidget(catalogueText2);
    catalogueLayout->addWidget(catalogueText3);
    catalogueLayout->addWidget(catalogueText4);
    catalogueLayout->addWidget(catalogueText5);

    centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(pushButtonArea); // 将切换按钮容器添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addStretch(); // 在布局的末尾添加一个弹性空间

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口中
}

SyncingPage::~SyncingPage()
{

}
