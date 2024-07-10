#ifndef MODIFYINFOR_WIN_H
#define MODIFYINFOR_WIN_H

#include <QWidget>
#include"ElaWidget.h"
#include <QLineEdit>
#include <QPushButton>
#include"user.h"
#include"ElaLineEdit.h"
#include"ElaPushButton.h"

class ElaInteractiveCard;
class QHBoxLayout;
class modifyInfor_win : public ElaWidget
{
    Q_OBJECT
public:
    modifyInfor_win(QWidget *parent = nullptr);
    ~modifyInfor_win();
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

#endif // MODIFYINFOR_WIN_H
