#include "signin.h"
#include "ui_signin.h"
#include<QMessageBox>

signin::signin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signin)
{
    ui->setupUi(this);
    ui->ID->setPlaceholderText("用户ID");
    ui->account->setPlaceholderText("账号");
    ui->password->setPlaceholderText("密码");
    ui->passagain->setPlaceholderText("再次确认密码");
}

signin::~signin()
{
    delete ui;
}

void signin::closeEvent(QCloseEvent *event)
{
    emit goback();
}

void signin::on_signinBtn_clicked()
{
    if(ui->ID->text()=="")
    {
        QMessageBox::information(this, "错误","请输入ID！");
    }
    else if(ui->account->text()=="")
    {
        QMessageBox::information(this, "错误","请输入账号！");
    }
    else if(ui->password->text()=="")
    {
        QMessageBox::information(this, "错误","请输入密码！");
    }
    else if(ui->passagain->text()=="")
    {
        QMessageBox::information(this, "错误","请再次确认密码！");
    }
    else if(ui->password->text()!=ui->passagain->text())
    {
        QMessageBox::information(this, "错误","两次输入的密码不一致！");
    }
    else
    {
        QMessageBox::information(this, "成功","注册成功");
        emit goback();
    }
}

