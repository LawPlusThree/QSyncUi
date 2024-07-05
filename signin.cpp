#include "signin.h"
#include "ui_signin.h"

signin::signin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::signin)
{
    ui->setupUi(this);
}

signin::~signin()
{
    delete ui;
}

void signin::closeEvent(QCloseEvent *event)
{

}

void signin::on_signinBtn_clicked()
{

}

