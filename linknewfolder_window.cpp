#include "linknewfolder_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

linkNewFolder_window::linkNewFolder_window(QWidget *parent)
    : ElaWidget(parent,600,400)
{
    this->setWindowTitle("珞珈云"); // 设置窗口标题
    this->setWindowModality(Qt::ApplicationModal); // 设置窗口模态
    this->hide(); // 初始隐藏当前窗口
    this->setWindowFlags(Qt::WindowMinimizeButtonHint); // 设置窗口标志，允许最小化
    QWidget*centerarea=new QWidget(); // 创建一个新的QWidget对象作为主区域
    QVBoxLayout* centerVLayout = new QVBoxLayout(centerarea); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(0, 0, 0, 0); // 设置布局边距,5为到右边的距离
    centerarea->setWindowTitle("链接新文件夹"); // 设置窗口标题

    this->setCentralWidget(centerarea);// 将中心部件添加到窗口
}

linkNewFolder_window::~linkNewFolder_window()
{

}
