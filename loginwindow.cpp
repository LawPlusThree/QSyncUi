#include "loginwindow.h"
#include "./ui_loginwindow.h"
#include<QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->account->setPlaceholderText("账号");
    ui->password->setPlaceholderText("密码");

    connect(resetp,&reset::goback,this,[=](){
        resetp->close();
        this->show();
    });
    connect(sign,&signin::goback,this,[=](){
        sign->close();
        this->show();
    });
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_loadBtn_clicked()
{
    if(ui->account->text()=="")
    {
        QMessageBox::information(this, "错误","请输入账号！");
    }
    else if(ui->password->text()=="")
    {
        QMessageBox::information(this, "错误","请输入密码！");
    }
    else if(ui->account->text()!=ui->password->text())
    {
        QMessageBox::information(this, "错误","账号或密码错误！");
    }
    else
    {
        this->close();
        //mainforms->show();
    }
}

void LoginWindow::on_resetBtn_clicked()
{
    this->hide();
    resetp->show();
}


void LoginWindow::on_signinBtn_clicked()
{
    this->hide();
    sign->show();
}

