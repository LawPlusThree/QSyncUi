//根据databasemanager中的数据库设置的更改信息界面，可以稍加修改来应用

#ifndef USERINFOPAGE_H
#define USERINFOPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include"user.h"
#include"ElaLineEdit.h"
#include"ElaPushButton.h"

class ElaInteractiveCard;
class QHBoxLayout;
class UserInfoPage : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfoPage(QWidget *parent = nullptr);
    User* currentUser;
    ElaInteractiveCard*avatar;
    QHBoxLayout*avatarArea;

private:
    ElaLineEdit *newIdEdit_;
    ElaLineEdit *newPasswordEdit_;
    ElaLineEdit *confirmNewPasswordEdit_;
    ElaPushButton *confirmButton_;

signals:
    void changexinxi(User user);

private slots:
    void onConfirmButtonClicked();
};

#endif // USERINFOPAGE_H
