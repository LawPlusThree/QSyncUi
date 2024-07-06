#ifndef SIGNINWIN_H
#define SIGNINWIN_H

#include"ElaWidget.h"
#include"ElaLineEdit.h"
#include "user.h"


class signinwin:public ElaWidget
{
    Q_OBJECT
public:
    explicit signinwin(QWidget *parent = nullptr);
    ~signinwin();

signals:
    void goback();
public:
    ElaLineEdit*IDLine;
    ElaLineEdit*accoutLine;
    ElaLineEdit*passwordLine;
    ElaLineEdit*passagainLine;
private slots:
    void on_signinBtn_clicked();
    void closeEvent(QCloseEvent*Event);
};

#endif // SIGNINWIN_H
