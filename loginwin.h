#ifndef LOGINWIN_H
#define LOGINWIN_H

#include"ElaWidget.h"
#include"resetwin.h"
#include"signinwin.h"
#include "mycore.h"
#include"databasemanager.h"

class ElaPushButton;
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
    DatabaseManager *db;

signals:
    void on_login_complete(User user);

private slots:
    void on_resetBtn_clicked();
    void on_signinBtn_clicked();
    void on_loginBtn_clicked();
    void on_accountLine_editingFinished();
private:
    Core *core;

};

#endif // LOGINWIN_H
