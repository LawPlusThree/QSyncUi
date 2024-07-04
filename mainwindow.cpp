#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->account->setPlaceholderText("账号");
    ui->password->setPlaceholderText("密码");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadBtn_clicked()
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

