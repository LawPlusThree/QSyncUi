#include "userinfopage.h"
#include <QVBoxLayout>
#include <QMessageBox>

UserInfoPage::UserInfoPage(DatabaseManager *dbManager, QWidget *parent) : QWidget(parent), dbManager_(dbManager)
{
    // 创建控件
    newIdEdit_ = new QLineEdit(this);
    newPasswordEdit_ = new QLineEdit(this);
    confirmNewPasswordEdit_ = new QLineEdit(this);
    confirmButton_ = new QPushButton("确认修改", this);

    // 设置密码输入为密码模式
    newPasswordEdit_->setEchoMode(QLineEdit::Password);
    confirmNewPasswordEdit_->setEchoMode(QLineEdit::Password);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(newIdEdit_);
    layout->addWidget(newPasswordEdit_);
    layout->addWidget(confirmNewPasswordEdit_);
    layout->addWidget(confirmButton_);

    // 连接信号与槽
    connect(confirmButton_, &QPushButton::clicked, this, &UserInfoPage::onConfirmButtonClicked);
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
    QString currentAccount = "currentUserAccount"; // 应替换为实际的当前用户账号
    if (dbManager_->updateUserInfo(currentAccount, newId, newPassword)) {
        QMessageBox::information(this, "成功", "用户信息更新成功。");
        // 更新主界面显示的用户ID
        // 假设主界面有一个label来显示用户ID，我们将更新它的文本
        //emit updateUserIdSignal(newId);
    } else {
        QMessageBox::warning(this, "错误", "用户信息更新失败。");
    }
}
