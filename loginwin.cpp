#include "loginwin.h"
#include"user.h"
#include"synctask.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>
#include<QPixmap>
#include<QBrush>
#include<QPalette>
#include<QColor>
#include<QMessageBox>

#include"ElaPushButton.h"
#include"ElaLineEdit.h"
#include"ElaText.h"
#include"ElaImageCard.h"

loginwin::loginwin(QWidget* parent):ElaWidget(parent)
{
    setWindowTitle("珞珈云");
    connect(resetWin,&resetwin::goback,this,[=](){
        resetWin->hide();
        this->show();
    });
    connect(signinWin,&signinwin::goback,this,[=](){
        signinWin->hide();
        this->show();
    });
    QWidget*area=new QWidget();
    this->setWindowModality(Qt::ApplicationModal);
    this->setCentralWidget(area);
    this->hide();
    this->setFixedSize(250,700);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);

    QImage image;
    ElaImageCard*avatar=new ElaImageCard();
    avatar->setCardImage(image);
    avatar->setFixedSize(150,150);
    QHBoxLayout*avatarArea=new QHBoxLayout();
    avatarArea->addWidget(avatar,0,Qt::AlignCenter);

    ElaText*ID=new ElaText("用户名");
    ID->setTextSize(16);
    QHBoxLayout*IDArea=new QHBoxLayout();
    IDArea->addWidget(ID,0,Qt::AlignCenter);

    accountLine=new ElaLineEdit();
    accountLine->setFixedSize(270,30);
    accountLine->setPlaceholderText("账号");
    QHBoxLayout*accountArea=new QHBoxLayout();
    accountArea->addWidget(accountLine,0,Qt::AlignCenter);

    passwordLine=new ElaLineEdit();
    passwordLine->setFixedSize(270,30);
    passwordLine->setPlaceholderText("密码");
    passwordLine->setEchoMode(QLineEdit::Password);
    QHBoxLayout*passwordArea=new QHBoxLayout();
    passwordArea->addWidget(passwordLine,0,Qt::AlignCenter);

    loginBtn=new ElaPushButton("登录");
    loginBtn->setFixedSize(270,30);
    loginBtn->setStyleSheet("background-color:rgb(0,204,255)");
    QHBoxLayout*loginBtnArea=new QHBoxLayout();
    loginBtnArea->addWidget(loginBtn,0,Qt::AlignCenter);

    ElaPushButton*signinBtn=new ElaPushButton("注册账号");
    ElaPushButton*resetBtn=new ElaPushButton("重置密码");
    signinBtn->setStyleSheet("background-color:transparent;border:none");
    resetBtn->setStyleSheet("background-color:transparent;border:none");
    QHBoxLayout*srArea=new QHBoxLayout();
    srArea->addWidget(signinBtn,0);
    srArea->addWidget(resetBtn,0);
    QHBoxLayout*srBtnArea=new QHBoxLayout();
    srBtnArea->addLayout(srArea,Qt::AlignCenter);

    QVBoxLayout*loginWinArea=new QVBoxLayout(area);
    loginWinArea->addLayout(avatarArea,Qt::AlignCenter);
    loginWinArea->addLayout(IDArea,Qt::AlignCenter);
    loginWinArea->addLayout(accountArea,Qt::AlignCenter);
    loginWinArea->addLayout(passwordArea,Qt::AlignCenter);
    loginWinArea->addLayout(loginBtnArea,Qt::AlignCenter);
    loginWinArea->addLayout(srBtnArea,Qt::AlignCenter);
    area->setLayout(loginWinArea);

    connect(resetBtn,&ElaPushButton::clicked,this, &loginwin::on_resetBtn_clicked);
    connect(signinBtn,&ElaPushButton::clicked,this, &loginwin::on_signinBtn_clicked);
    connect(loginBtn,&ElaPushButton::clicked,this, &loginwin::on_loginBtn_clicked);
}

loginwin::~loginwin()
{

}

void loginwin::on_resetBtn_clicked()
{
    resetWin->show();
}

void loginwin::on_signinBtn_clicked()
{
    signinWin->show();
}

void loginwin::on_loginBtn_clicked()
{
    loginBtn->setEnabled(false);
    if(accountLine->text()=="")
    {
        QMessageBox::information(this, "错误","请输入账号！");
    }
    else if(passwordLine->text()=="")
    {
        QMessageBox::information(this, "错误","请输入密码！");
    }
    else
    {
        //测试用户
        //User loginuser("newuser@example.com","123456");
        User loginuser(accountLine->text(),passwordLine->text());
        if(loginuser.login()){
            QMessageBox::information(this, "成功","登录成功");
            emit on_login_complete(loginuser);
            this->close();
        }
        else
            QMessageBox::critical(this, "失败","登录失败");
    }
    loginBtn->setEnabled(true);
}

