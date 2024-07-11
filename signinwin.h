#ifndef SIGNINWIN_H
#define SIGNINWIN_H

#include"ElaWidget.h"
#include"ElaLineEdit.h"
#include "user.h"

class ElaInteractiveCard;
class QHBoxLayout;
class ElaPushButton;
class signinwin:public ElaWidget
{
    Q_OBJECT
public:
    explicit signinwin(QWidget *parent = nullptr);
    ~signinwin();
    ElaPushButton*signinBtn;

signals:
    void goback();
    void on_signin_complete(User user);

public:
    ElaInteractiveCard*avatar;
    QHBoxLayout*avatarArea;
    ElaLineEdit*IDLine;
    ElaLineEdit*accoutLine;
    ElaLineEdit*passwordLine;
    ElaLineEdit*passagainLine;
    QString avatarDir="";
private slots:
    void on_signinBtn_clicked();
    void closeEvent(QCloseEvent*Event);
    void on_image_clicked();
};

#endif // SIGNINWIN_H
