#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include "user.h"

namespace Ui {
class signin;
}

class signin : public QWidget
{
    Q_OBJECT

public:
    explicit signin(QWidget *parent = nullptr);
    ~signin();

signals:
    void goback();
private slots:
    void closeEvent(QCloseEvent *event);

    void on_signinBtn_clicked();

private:
    Ui::signin *ui;
};

#endif // SIGNIN_H
