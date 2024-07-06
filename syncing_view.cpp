#include "syncing_view.h"
#include "ElaText.h"
#include <QVBoxLayout>

SyncingPage::SyncingPage(QWidget* parent)
    : ElaScrollPage(parent)
{
    QWidget* centralWidget = new QWidget(this); // 创建中心部件
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(0, 0, 5, 0); // 设置布局边距,5为到右边的距离
    centralWidget->setWindowTitle("正在同步"); // 设置窗口标题

    ElaText* iconTitle = new ElaText("On this page, you can use the search function to use your favorite icons in the program", this);
    iconTitle->setTextSize(15); // 设置文字大小

    // 创建一个 ElaProgressBar 对象，用作进度条
    _progressBar = new ElaProgressBar(this);
    // 设置QProgressBar的大小策略为水平方向可扩展，垂直方向固定
    _progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 设置最小尺寸，可以适当调整宽度以减少填充效果
    _progressBar->setMinimumSize(100, 20); // 调整最小宽度为100，最小高度20
    // 设置进度条的最小值和最大值
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0); // 表示不确定的进度

    ElaScrollPageArea* upBarArea = new ElaScrollPageArea(this);//上方固定区域
    QVBoxLayout* upBarLayout = new QVBoxLayout(upBarArea);//上方固定区域垂直布局
    upBarLayout->setContentsMargins(0, 0, 0, 0);//设置布局的边距

    // 创建一个 ElaScrollPageArea 对象，用作进度条的容器
    ElaScrollPageArea* progressBarArea = new ElaScrollPageArea(upBarArea);
    // 为进度条容器设置水平布局
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    progressBarLayout->setContentsMargins(50, 0, 50, 0); // 设置布局的边距
    // 创建一个 ElaText 对象，用于显示进度条的文本标签
    ElaText* progressBarText = new ElaText("总进度：", this);
    // 设置进度条文本标签的字体大小为 15
    progressBarText->setTextSize(15);
    // 将进度条文本标签和进度条控件添加到布局中
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);

    ElaText* catalogueText = new ElaText("这是目录", this);//创建一个ElaText对象，用于显示目录文本
    catalogueText->setTextSize(15); // 设置文字大小

    upBarLayout->addWidget(progressBarArea);//将进度条容器添加到上方固定区域
    upBarLayout->addWidget(catalogueText);//将目录文本添加到上方固定区域

    centerVLayout->addWidget(upBarArea); // 将 progressBarArea 添加到布局中
    centerVLayout->addWidget(iconTitle);// 将字添加到布局中

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口中
}

SyncingPage::~SyncingPage()
{

}
