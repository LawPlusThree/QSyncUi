#include "modifyinfor_win.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include<QFileDialog>
#include"ElaInteractiveCard.h"


modifyInfor_win::modifyInfor_win(QWidget *parent)
    : ElaWidget(parent,400,500)
{
    this->setWindowButtonFlag(ElaAppBarType::MaximizeButtonHint,false);
    this->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);
    setWindowTitle("珞珈云");
    QWidget*area=new QWidget();
    this->setWindowModality(Qt::ApplicationModal);
    this->setCentralWidget(area);
    this->hide();
    this->setFixedSize(250,600);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);

    // 创建控件
    ElaInteractiveCard*avatar=new ElaInteractiveCard();
    avatar->setFixedSize(150,150);
    avatar->setCardPixmapSize(140,140);
    avatar->setCardPixmapBorderRadius(70);
    QPixmap image(":\\avatarImage\\AvatarImage\\setAvatar.png");
    avatar->setCardPixmap(image);
    avatarArea=new QHBoxLayout();
    avatarArea->addWidget(avatar,0,Qt::AlignCenter);

    newIdEdit_ = new ElaLineEdit(this);
    newIdEdit_->setFixedSize(270,30);
    QHBoxLayout*IDArea=new QHBoxLayout();
    IDArea->addWidget(newIdEdit_,0,Qt::AlignCenter);

    newPasswordEdit_ = new ElaLineEdit(this);
    newPasswordEdit_->setFixedSize(270,30);
    QHBoxLayout*PasswordArea=new QHBoxLayout();
    PasswordArea->addWidget(newPasswordEdit_,0,Qt::AlignCenter);

    confirmNewPasswordEdit_ = new ElaLineEdit(this);
    confirmNewPasswordEdit_->setFixedSize(270,30);
    QHBoxLayout*ConfirmArea=new QHBoxLayout();
    ConfirmArea->addWidget(confirmNewPasswordEdit_,0,Qt::AlignCenter);

    confirmButton_ = new ElaPushButton("确认修改", this);
    confirmButton_->setFixedSize(270,30);
    confirmButton_->setStyleSheet("background-color:rgb(0,204,255)");
    QHBoxLayout*confirmBtnArea=new QHBoxLayout();
    confirmBtnArea->addWidget(confirmButton_,0,Qt::AlignCenter);

    // 设置密码输入为密码模式
    newPasswordEdit_->setEchoMode(ElaLineEdit::Password);
    confirmNewPasswordEdit_->setEchoMode(ElaLineEdit::Password);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    /*layout->addWidget(newIdEdit_);
    layout->addWidget(newPasswordEdit_);
    layout->addWidget(confirmNewPasswordEdit_);
    layout->addWidget(confirmButton_);*/
    layout->addLayout(avatarArea);
    layout->addLayout(IDArea);
    layout->addLayout(PasswordArea);
    layout->addLayout(ConfirmArea);
    layout->addLayout(confirmBtnArea);


    // 获取当前登录的用户信息
    //QString userEmail = currentUser->getEmail();
    newIdEdit_->setPlaceholderText("修改ID");
    newPasswordEdit_->setPlaceholderText("新密码");
    confirmNewPasswordEdit_->setPlaceholderText("确认密码");

    // 设置样式
    /*QString styleSheet = "QLineEdit { background-color: #e5e5e5; border: 1px solid #ccc; padding: 5px; }";
    newIdEdit_->setStyleSheet(styleSheet);
    newPasswordEdit_->setStyleSheet(styleSheet);
    confirmNewPasswordEdit_->setStyleSheet(styleSheet);*/

    // 重新设置布局边距和间距
    //layout->setContentsMargins(40, 20, 40, 20);
    //layout->setSpacing(10);

    // 连接信号与槽
    connect(confirmButton_, &ElaPushButton::clicked, this, &modifyInfor_win::onConfirmButtonClicked);
    connect(avatar,&ElaInteractiveCard::clicked,this,&modifyInfor_win::onAvatarClicked);
    area->setLayout(layout);
}

modifyInfor_win::~modifyInfor_win()
{
}

//被展示的时候
void modifyInfor_win::showEvent(QShowEvent *event)
{
    newIdEdit_->setText(currentUser->getUsername());
    newPasswordEdit_->setText(currentUser->gethashedPassword());
    confirmNewPasswordEdit_->setText(currentUser->gethashedPassword());
}

void modifyInfor_win::onConfirmButtonClicked()
{
    QString newId = newIdEdit_->text();
    QString newPassword = newPasswordEdit_->text();
    QString confirmNewPassword = confirmNewPasswordEdit_->text();
    confirmButton_->setEnabled(false);
    confirmButton_->setText("修改中......");
    confirmButton_->setStyleSheet("background-color:rgb(138,151,123)");

    // 简单的验证
    if (newId.isEmpty() || newPassword.isEmpty() || confirmNewPassword.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写完所有信息。");
        confirmButton_->setEnabled(true);
        confirmButton_->setText("确认修改");
        confirmButton_->setStyleSheet("background-color:rgb(0,204,255)");
        return;
    }

    if (newPassword != confirmNewPassword) {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致。");
        confirmButton_->setEnabled(true);
        confirmButton_->setText("确认修改");
        confirmButton_->setStyleSheet("background-color:rgb(0,204,255)");
        return;
    }
    currentUser->username = newId;
    currentUser->hashedPassword = newPassword;
    currentUser->updateUser();
    currentUser->updateAvatar(avatarPath);
    // 更新数据库
    currentUser->username = newId;
    currentUser->hashedPassword = newPassword;
    emit changexinxi(User(*currentUser));
    confirmButton_->setEnabled(true);
    confirmButton_->setText("确认修改");
    confirmButton_->setStyleSheet("background-color:rgb(0,204,255)");
    QMessageBox::information(this, "成功","修改成功");
    confirmButton_->setEnabled(true);
    confirmButton_->setText("确认修改");
    confirmButton_->setStyleSheet("background-color:rgb(0,204,255)");
    this->hide();
}

void modifyInfor_win::onAvatarClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图像文件 (*.png *.jpg *.jpeg *.bmp)"));
    fileName = QDir::toNativeSeparators(fileName);
    qDebug() << "Selected file path: " << fileName;
    if (!fileName.isEmpty())
    {
        QPixmap pix(fileName);
        if (pix.isNull())
        {
            // 处理加载失败的情况
            return;
        }
        while (QLayoutItem* item = avatarArea->takeAt(0)) {
            delete item;
        }
        avatar=new ElaInteractiveCard();
        avatar->setCardPixmap(pix);
        avatar->setFixedSize(150,150);
        avatar->setCardPixmapSize(140,140);
        avatar->setCardPixmapBorderRadius(70);
        avatarArea->addWidget(avatar,0,Qt::AlignCenter);
        update();
        avatarPath=fileName;
    }
}
