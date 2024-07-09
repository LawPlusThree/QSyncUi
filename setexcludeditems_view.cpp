#include "setexcludeditems_view.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ElaText.h"
#include "ElaContentDialog.h"
#include <QTextEdit>
#include "ElaPushButton.h"

setExcludedItems_view::setExcludedItems_view(QWidget *parent)
    : ElaWidget(parent,700,400)
{
    this->setWindowTitle("珞珈云"); // 设置窗口标题
    this->setWindowModality(Qt::ApplicationModal); // 设置窗口模态
    this->hide(); // 初始隐藏当前窗口
    this->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);
    QWidget*centerarea=new QWidget(); // 创建一个新的QWidget对象作为主区域
    QVBoxLayout* centerVLayout = new QVBoxLayout(centerarea); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(60, 15, 60, 30); // 设置布局边距

    ElaText* Title = new ElaText("设置排除项目", this);
    Title->setTextSize(25); // 设置文字大小

    QTextEdit* textEdit = new QTextEdit(this);
    textEdit->setPlaceholderText("输入示例。。。。。。"); // 设置输入提示文本

    QWidget* buttonArea = new QWidget();
    buttonArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    buttonArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    QHBoxLayout* buttonLayout = new QHBoxLayout(buttonArea);// 创建一个水平布局
    buttonLayout->setContentsMargins(0, 0, 0, 0); // 设置布局的边距
    _pushButton2 = new ElaPushButton("取消", this);
    _pushButton2->setFixedSize(120, 40);
    _pushButton3 = new ElaPushButton("确认", this);
    _pushButton3->setFixedSize(120, 40);
    buttonLayout->addWidget(_pushButton2);
    buttonLayout->addWidget(_pushButton3);

    centerVLayout->addWidget(Title);
    centerVLayout->addWidget(textEdit);
    centerVLayout->addWidget(buttonArea);

    this->setCentralWidget(centerarea);// 将中心部件添加到窗口
}

setExcludedItems_view::~setExcludedItems_view()
{

}
