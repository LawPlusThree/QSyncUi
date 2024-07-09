#include "userinfopage.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include"ElaInteractiveCard.h"

UserInfoPage::UserInfoPage(QWidget *parent) : QWidget(parent)
{
    // 创建控件
    ElaInteractiveCard*avatar=new ElaInteractiveCard();
    avatar->setFixedSize(200,200);
    avatar->setCardPixmapSize(180,180);
    avatar->setCardPixmapBorderRadius(90);
    QPixmap image(":\\avatarImage\\AvatarImage\\setAvatar.png");
    avatar->setCardPixmap(image);
    avatarArea=new QHBoxLayout();
    avatarArea->addWidget(avatar,0,Qt::AlignCenter);

    newIdEdit_ = new ElaLineEdit(this);
    newIdEdit_->setFixedSize(360,40);
    QHBoxLayout*IDArea=new QHBoxLayout();
    IDArea->addWidget(newIdEdit_,0,Qt::AlignCenter);

    newPasswordEdit_ = new ElaLineEdit(this);
    newPasswordEdit_->setFixedSize(360,40);
    QHBoxLayout*PasswordArea=new QHBoxLayout();
    PasswordArea->addWidget(newPasswordEdit_,0,Qt::AlignCenter);

    confirmNewPasswordEdit_ = new ElaLineEdit(this);
    confirmNewPasswordEdit_->setFixedSize(360,40);
    QHBoxLayout*ConfirmArea=new QHBoxLayout();
    ConfirmArea->addWidget(confirmNewPasswordEdit_,0,Qt::AlignCenter);

    confirmButton_ = new ElaPushButton("确认修改", this);
    confirmButton_->setFixedSize(360,40);
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
    layout->setContentsMargins(40, 20, 40, 20);
    layout->setSpacing(10);

    // 连接信号与槽
    connect(confirmButton_, &ElaPushButton::clicked, this, &UserInfoPage::onConfirmButtonClicked);
}

void UserInfoPage::onConfirmButtonClicked()
{
    QString newId = newIdEdit_->text();
    QString newPassword = newPasswordEdit_->text();
    QString confirmNewPassword = confirmNewPasswordEdit_->text();

    // 简单的验证
    if (newId.isEmpty() || newPassword.isEmpty() || confirmNewPassword.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写完所有信息。");
        return;
    }

    if (newPassword != confirmNewPassword) {
        QMessageBox::warning(this, "错误", "两次输入的密码不一致。");
        return;
    }

    // 更新数据库
    emit changexinxi(User(*currentUser));

    QMessageBox::information(this, "成功","修改成功");
}
