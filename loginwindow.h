#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include"reset.h"
#include"signin.h"
#include <QMainWindow>
#include "mycore.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    reset*resetp=new reset();
    signin*sign=new signin();

private slots:
    void on_loadBtn_clicked();

    void on_resetBtn_clicked();

    void on_signinBtn_clicked();

private:
    Core *core;
    Ui::MainWindow *ui;
};
#endif // LOGINWINDOW_H
