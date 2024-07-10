#include "linknewfolder_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QRegularExpression>
#include <QMessageBox>
#include "ElaCheckBox.h"
#include "ElaLineEdit.h"
#include "ElaPushButton.h"
#include "ElaText.h"
#include "ElaComboBox.h"
#include "ElaMessageBar.h"
#include "synctask.h"

linkNewFolder_window::linkNewFolder_window(QWidget *parent)
    : ElaWidget(parent,900,400)
{
    this->setWindowTitle("珞珈云"); // 设置窗口标题
    this->setWindowModality(Qt::ApplicationModal); // 设置窗口模态
    this->hide(); // 初始隐藏当前窗口
    this->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);
    this->setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint,false);
    this->setWindowButtonFlag(ElaAppBarType::MaximizeButtonHint,false);
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
    folderName1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    folderName1->setFixedHeight(40); // 固定高度为40
    _pushButton1 = new ElaPushButton("选择文件夹", this);
    _pushButton1->setFixedSize(130, 40); // 设置按钮的固定大小
    lineEditLayout1->addWidget(folderName1, 1); // 将输入框添加到布局中，并设置拉伸系数为1
    lineEditLayout1->addWidget(_pushButton1); // 将按钮添加到布局中
    connect(_pushButton1, &ElaPushButton::clicked, [=]() {
        QString folderPath = QFileDialog::getExistingDirectory(this, "选择文件夹", QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!folderPath.isEmpty()) {
            folderName1->setText(folderPath);
        }
    });

    QWidget* lineEditArea2 = new QWidget();
    lineEditArea2->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    lineEditArea2->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    QHBoxLayout* lineEditLayout2 = new QHBoxLayout(lineEditArea2);// 创建一个水平布局
    lineEditLayout2->setContentsMargins(0, 0, 0, 0); // 设置布局的边距
    ElaLineEdit* folderName2 = new ElaLineEdit(this); // 创建一个 ElaLineEdit 对象
    folderName2->setPlaceholderText("云端文件夹地址(只能包含大小写字母、数字、'-'和'/'，且必须以'/'结尾)"); // 设置输入提示文本
    folderName2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    folderName2->setFixedHeight(40); // 固定高度为40
    _checkBox = new ElaCheckBox("使用原文件夹名", this);
    _checkBox->setFixedSize(130, 20); // 设置按钮的固定大小
    lineEditLayout2->addWidget(folderName2, 1); // 将输入框添加到布局中
    lineEditLayout2->addWidget(_checkBox); // 将按钮添加到布局中
    // 首先，定义一个用于显示错误信息的标签
    QLabel* errorLabel = new QLabel(lineEditArea2);
    errorLabel->setText("输入格式错误：只能包含大小写字母、数字、'-'和'/'，且必须以'/'结尾");
    errorLabel->setStyleSheet("QLabel { color : red; }"); // 设置错误信息为红色字体
    //errorLabel->setAlignment(Qt::AlignCenter); // 设置文本居中显示
    errorLabel->setFixedSize(500,20);
    errorLabel->hide(); // 默认隐藏错误信息标签
    // 然后，为folderName2添加失焦事件的处理
    connect(folderName2, &ElaLineEdit::focusOut, [=]() {
        QString inputText = folderName2->text();
        QRegularExpression re("^[a-zA-Z0-9/-]+/$");
        if (!re.match(inputText).hasMatch()) {
            // 如果输入不符合要求，显示错误信息标签
            errorLabel->show();
        } else {
            // 如果输入符合要求，隐藏错误信息标签
            errorLabel->hide();
        } 
    });
    // 为checkBox添加状态改变事件的处理
    connect(_checkBox, &ElaCheckBox::stateChanged, [=](int state) {
        if (state == Qt::Checked) {
            QString folderName1Text = folderName1->text();
            int lastSlashIndex = folderName1Text.lastIndexOf('/');
            // 如果找到了'/'，则复制其后的内容到folderName2
            if (lastSlashIndex != -1) {
                QString lastPart = folderName1Text.mid(lastSlashIndex + 1); // 包含最后一个'/'及其后的所有内容
                folderName2->setText(lastPart + "/"); // 在folderName2中设置文本，并在末尾加上一个'/'
                QString inputText = folderName2->text();
                QRegularExpression re("^[a-zA-Z0-9/-]+/$");
                if (!re.match(inputText).hasMatch())
                    errorLabel->show();
                else
                    errorLabel->hide();
            }
        }
    });

    _comboBox = new ElaComboBox(this);
    QStringList comboList{"仅上传", "仅下载", "同步上传与下载"};
    _comboBox->addItems(comboList);
    _comboBox->setFixedWidth(140);
    // 创建一个映射，将文本选项映射到数字
    QMap<QString, int> comboOptionToNumber;
    comboOptionToNumber["仅上传"] = 2;
    comboOptionToNumber["仅下载"] = 3;
    comboOptionToNumber["同步上传与下载"] = 1;

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
    connect(_pushButton2,&ElaPushButton::clicked,[=](){
        this->hide();
    });
    connect(_pushButton3, &ElaPushButton::clicked, [=]() {
        QString inputText = folderName2->text();
        QRegularExpression re("^[a-zA-Z0-9/-]+/$");

        if (!re.match(inputText).hasMatch()) {
            // 如果输入不符合要求，显示警告对话框
            //QMessageBox::warning(this, "输入错误", "云端文件夹地址只能包含大小写字母、数字、'-'和'/'，且必须以'/'结尾。");
            ElaMessageBar::error(ElaMessageBarType::TopRight,"输入错误", "云端文件夹地址输入错误，只能包含大小写字母、数字、'-'和'/'，且必须以'/'结尾。", 2000,this);
        } else {
            // 如果输入符合要求，执行后续操作
            this->hide();
            ElaMessageBar::success(ElaMessageBarType::TopRight,"链接成功", "新文件夹链接成功。", 2000);
            qDebug() << "本地文件夹地址：" << folderName1->text();
            qDebug() << "云端文件夹地址：" << folderName2->text();
            int syncOption = comboOptionToNumber[_comboBox->currentText()];
            qDebug() << "同步方式：" << syncOption;//["仅上传"] = 2;["仅下载"] = 3;["同步上传与下载"] = 1;
            SyncTask task(folderName1->text(), folderName2->text(), syncOption);
            emit onNewTask(task);
        }
    });


    centerVLayout->addWidget(Title);
    centerVLayout->addWidget(lineEditArea1);
    //centerVLayout->addWidget(errorLabel);
    centerVLayout->addWidget(lineEditArea2);
    centerVLayout->addWidget(_comboBox);
    centerVLayout->addWidget(buttonArea);

    this->setCentralWidget(centerarea);// 将中心部件添加到窗口
}

linkNewFolder_window::~linkNewFolder_window()
{

}
