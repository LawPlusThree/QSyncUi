#include "userinfopage.h"
#include <QVBoxLayout>
#include <QMessageBox>


UserInfoPage::UserInfoPage(QWidget *parent) : QWidget(parent)
{
    // 创建控件
    newIdEdit_ = new ElaLineEdit(this);
    newPasswordEdit_ = new ElaLineEdit(this);
    confirmNewPasswordEdit_ = new ElaLineEdit(this);
    confirmButton_ = new ElaPushButton("确认修改", this);

    // 设置密码输入为密码模式
    newPasswordEdit_->setEchoMode(ElaLineEdit::Password);
    confirmNewPasswordEdit_->setEchoMode(ElaLineEdit::Password);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(newIdEdit_);
    layout->addWidget(newPasswordEdit_);
    layout->addWidget(confirmNewPasswordEdit_);
    layout->addWidget(confirmButton_);

    // 获取当前登录的用户信息
    //QString userEmail = currentUser->getEmail();
    newIdEdit_->setPlaceholderText("修改ID");
    newPasswordEdit_->setPlaceholderText("新密码");
    confirmNewPasswordEdit_->setPlaceholderText("确认密码");


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
