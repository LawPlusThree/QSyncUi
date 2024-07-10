#ifndef LOGINWIN_H
#define LOGINWIN_H

#include"ElaWidget.h"
#include"resetwin.h"
#include"signinwin.h"
#include "messagechannel.h"
class ElaPushButton;
class ElaInteractiveCard;
class QHBoxLayout;
class loginwin:public ElaWidget
{
    Q_OBJECT
public:
    loginwin(QWidget *parent = nullptr);
    ~loginwin();
    resetwin*resetWin=new resetwin();
    signinwin*signinWin=new signinwin();

    ElaLineEdit*accountLine;
    ElaLineEdit*passwordLine;
    ElaPushButton*loginBtn;
    MessageChannel *channel;
    ElaInteractiveCard*avatar;
    QHBoxLayout*avatarArea;

signals:
    void on_login_complete(User user);
    void needPassword(const QString & account);

public slots:
    void on_db_response(const QString & password);
private slots:
    void on_resetBtn_clicked();
    void on_signinBtn_clicked();
    void on_loginBtn_clicked();
    void on_accountLine_editingFinished();


};

#endif // LOGINWIN_H
