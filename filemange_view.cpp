#include "filemange_view.h"

#include<QHBoxLayout>
#include"ElaWidget.h"
#include"ElaText.h"
#include"ElaPushButton.h"
#include"ElaToggleButton.h"
#include"ElaScrollArea.h"

#include"DirCard.h"

FileManagePage::FileManagePage(QWidget* parent):ElaScrollPage(parent)
{
    /*ElaWidget*topCard=new ElaWidget();
    topCard->setMaximumHeight(50);
    QVBoxLayout*topCardArea=new QVBoxLayout(topCard);*/

    QWidget* centralWidget = new QWidget(this); // 创建中心部件
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(0, 0, 5, 0); // 设置布局边距,5为到右边的距离
    centralWidget->setWindowTitle("同步文件夹管理"); // 设置窗口标题

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
    _pushButton1 = new ElaPushButton("解除绑定", this);
    _pushButton1->setFixedSize(100, 40); // 设置按钮的固定大小
    _toggleButton = new ElaToggleButton("暂时停止", this);
    _toggleButton->setFixedSize(100, 40); // 设置按钮的固定大小
    _pushButton2 = new ElaPushButton("设置排除项目", this);
    _pushButton2->setFixedSize(100, 40); // 设置按钮的固定大小
    _pushButton3 = new ElaPushButton("链接新文件夹", this);
    _pushButton3->setFixedSize(100, 40); // 设置按钮的固定大小
    QWidget* pushButtonArea = new QWidget();
    pushButtonArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    pushButtonArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // 创建一个 ElaScrollPageArea 对象，用作切换按钮的容器，并指定其父对象
    //ElaScrollPageArea* pushButtonArea = new ElaScrollPageArea(this);
    // 为切换按钮容器设置水平布局
    QHBoxLayout* pushButtonLayout = new QHBoxLayout(pushButtonArea);
    pushButtonLayout->setContentsMargins(50, 0, 55, 0);
    // 将切换按钮控件添加到布局中
    pushButtonLayout->addWidget(_toggleButton);
    pushButtonLayout->addWidget(_pushButton1);
    pushButtonLayout->addWidget(_pushButton2);
    pushButtonLayout->addWidget(_pushButton3);
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

    // 创建QScrollArea用于包裹files
    ElaScrollArea* scrollArea = new ElaScrollArea();
    scrollArea->viewport()->setStyleSheet("background:transparent;");//设置背景透明
    DirCard*DirCardArea1=new DirCard("文件1","1.0GB","2024.7.1");
    DirCard*DirCardArea2=new DirCard("文件2","2.0GB","2024.7.2");
    DirCard*DirCardArea3=new DirCard("文件3","3.0GB","2024.7.3");
    DirCard*DirCardArea4=new DirCard("文件4","4.0GB","2024.7.4");
    DirCard*DirCardArea5=new DirCard("文件5","5.0GB","2024.7.5");
    DirCard*DirCardArea6=new DirCard("文件6","6.0GB","2024.7.6");
    DirCard*DirCardArea7=new DirCard("文件7","7.0GB","2024.7.7");
    DirCard*DirCardArea8=new DirCard("文件8","8.0GB","2024.7.8");
    DirCard*DirCardArea9=new DirCard("文件9","9.0GB","2024.7.9");
    DirCard*DirCardArea10=new DirCard("文件10","10.0GB","2024.7.10");
    QWidget* filesWidget=new QWidget();
    QVBoxLayout* filesLayout=new QVBoxLayout(filesWidget);
    filesLayout->addWidget(DirCardArea1);
    filesLayout->addWidget(DirCardArea2);
    filesLayout->addWidget(DirCardArea3);
    filesLayout->addWidget(DirCardArea4);
    filesLayout->addWidget(DirCardArea5);
    filesLayout->addWidget(DirCardArea6);
    filesLayout->addWidget(DirCardArea7);
    filesLayout->addWidget(DirCardArea8);
    filesLayout->addWidget(DirCardArea9);
    filesLayout->addWidget(DirCardArea10);
    filesLayout->setAlignment(Qt::AlignTop);
    scrollArea->setWidget(filesWidget); // 设置scrollArea的内容部件
    scrollArea->setWidgetResizable(true); // 允许scrollArea根据内容自动调整大小

    centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(pushButtonArea); // 将切换按钮容器添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addWidget(scrollArea); // 将scrollArea添加到布局中
    //centerVLayout->addStretch(); // 在布局的末尾添加一个弹性空间

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口

}
