#include "linknewfolder_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "ElaCheckBox.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"
#include "ElaComboBox.h"

linkNewFolder_window::linkNewFolder_window(QWidget *parent)
    : ElaWidget(parent,700,400)
{
    this->setWindowTitle("珞珈云"); // 设置窗口标题
    this->setWindowModality(Qt::ApplicationModal); // 设置窗口模态
    this->hide(); // 初始隐藏当前窗口
    this->setWindowFlags(Qt::WindowMinimizeButtonHint); // 设置窗口标志，允许最小化
    QWidget*centerarea=new QWidget(); // 创建一个新的QWidget对象作为主区域
    QVBoxLayout* centerVLayout = new QVBoxLayout(centerarea); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(60, 0, 60, 30); // 设置布局边距

    ElaText* Title = new ElaText("链接新文件夹", this);
    Title->setTextSize(25); // 设置文字大小

    QWidget* lineEditArea1 = new QWidget();
    lineEditArea1->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    lineEditArea1->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    QHBoxLayout* lineEditLayout1 = new QHBoxLayout(lineEditArea1);// 创建一个水平布局
    lineEditLayout1->setContentsMargins(0, 0, 0, 0); // 设置布局的边距
    ElaLineEdit* folderName1 = new ElaLineEdit(this); // 创建一个 ElaLineEdit 对象
    folderName1->setPlaceholderText("本地文件夹地址"); // 设置输入提示文本
    folderName1->setFixedSize(450, 40); // 设置控件的固定大小
    _pushButton1 = new ElaPushButton("选择文件", this);
    _pushButton1->setFixedSize(120, 40); // 设置按钮的固定大小
    lineEditLayout1->addWidget(folderName1); // 将输入框添加到布局中
    lineEditLayout1->addStretch();
    lineEditLayout1->addWidget(_pushButton1); // 将按钮添加到布局中

    QWidget* lineEditArea2 = new QWidget();
    lineEditArea2->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    lineEditArea2->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    QHBoxLayout* lineEditLayout2 = new QHBoxLayout(lineEditArea2);// 创建一个水平布局
    lineEditLayout2->setContentsMargins(0, 0, 0, 0); // 设置布局的边距
    ElaLineEdit* folderName2 = new ElaLineEdit(this); // 创建一个 ElaLineEdit 对象
    folderName2->setPlaceholderText("云端文件夹地址"); // 设置输入提示文本
    folderName2->setFixedSize(450, 40); // 设置控件的固定大小
    _checkBox = new ElaCheckBox("使用原目录名", this);
    _checkBox->setFixedSize(120, 20); // 设置按钮的固定大小
    lineEditLayout2->addWidget(folderName2); // 将输入框添加到布局中
    lineEditLayout2->addStretch();
    lineEditLayout2->addWidget(_checkBox); // 将按钮添加到布局中

    _comboBox = new ElaComboBox(this);
    QStringList comboList{"仅上传", "仅下载", "同步上传与下载"};
    _comboBox->addItems(comboList);
    _comboBox->setFixedWidth(140);

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
    centerVLayout->addWidget(lineEditArea1);
    centerVLayout->addWidget(lineEditArea2);
    centerVLayout->addWidget(_comboBox);
    centerVLayout->addWidget(buttonArea);

    this->setCentralWidget(centerarea);// 将中心部件添加到窗口
}

linkNewFolder_window::~linkNewFolder_window()
{

}
