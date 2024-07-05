#include "reset.h"
#include "ui_reset.h"
#include<QMessageBox>

reset::reset(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::reset)
{
    ui->setupUi(this);
    ui->Password->setPlaceholderText("新密码");
    ui->Passagain->setPlaceholderText("再次确认密码");
}

reset::~reset()
{
    delete ui;
}

void reset::on_resetBtn_clicked()
{
    if(ui->Password->text()=="")
    {
        QMessageBox::information(this, "错误","请输入新密码！");
    }
    else if(ui->Passagain->text()=="")
    {
        QMessageBox::information(this, "错误","请再次确认密码！");
    }
    else if(ui->Password->text()!=ui->Passagain->text())
    {
        QMessageBox::information(this, "错误","两次输入的密码不一致！");
    }
    else
    {
        QMessageBox::information(this, "成功","重置成功");
        emit goback();
    }
}

void reset::closeEvent(QCloseEvent *event)
{
    emit goback();
}

