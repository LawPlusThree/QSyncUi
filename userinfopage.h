//根据databasemanager中的数据库设置的更改信息界面，可以稍加修改来应用

#ifndef USERINFOPAGE_H
#define USERINFOPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "databasemanager.h"

class UserInfoPage : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfoPage(DatabaseManager *dbManager, QWidget *parent = nullptr);

private:
    DatabaseManager *dbManager_;
    QLineEdit *newIdEdit_;
    QLineEdit *newPasswordEdit_;
    QLineEdit *confirmNewPasswordEdit_;
    QPushButton *confirmButton_;

private slots:
    void onConfirmButtonClicked();
};

#endif // USERINFOPAGE_H
