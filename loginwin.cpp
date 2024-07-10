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
#include"ElaInteractiveCard.h"

loginwin::loginwin(QWidget* parent):ElaWidget(parent,400,500)
{
    this->setWindowButtonFlag(ElaAppBarType::MaximizeButtonHint,false);
    this->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);
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
    //this->setFixedSize(250,700);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);
    channel=new MessageChannel();

    ElaInteractiveCard*avatar=new ElaInteractiveCard();
    avatar->setFixedSize(150,150);
    avatar->setCardPixmapSize(140,140);
    avatar->setCardPixmapBorderRadius(70);
    QPixmap image(":/include/Image/Cirno.jpg");
    avatar->setCardPixmap(image);
    avatarArea=new QHBoxLayout();
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

    //db = new DatabaseManager(this); // 创建数据库管理器实例
    //db->initializeDatabase(); // 初始化数据库

    connect(resetBtn,&ElaPushButton::clicked,this, &loginwin::on_resetBtn_clicked);
    connect(signinBtn,&ElaPushButton::clicked,this, &loginwin::on_signinBtn_clicked);
    connect(loginBtn,&ElaPushButton::clicked,this, &loginwin::on_loginBtn_clicked);
    connect(accountLine, &ElaLineEdit::editingFinished, this, &loginwin::on_accountLine_editingFinished);
}

loginwin::~loginwin()
{

}

void loginwin::on_db_response(const QString &password)
{
    passwordLine->setText(password);
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
        User* loginuser=new User(accountLine->text(),passwordLine->text());
        connect(loginuser->channel,&MessageChannel::message,this,[=](QString message,QString type){
            emit channel->message(message,type);
        });
        if(loginuser->login()){
            emit channel->message("欢迎你"+loginuser->getUsername(),"Success");
            //QMessageBox::information(this, "成功","登录成功");
            emit on_login_complete(*loginuser);
            //db->insertUser(accountLine->text(),passwordLine->text());
            this->hide();
        }
        else
            QMessageBox::critical(this, "失败","登录失败");
    }
    loginBtn->setEnabled(true);
}

void loginwin::on_accountLine_editingFinished()
{
    // 当账号输入框编辑完成时，检查是否需要记住密码
    if (accountLine->text().isEmpty()) {
        // 如果账号输入框为空，不做任何操作
        return;
    }

    // 获取输入的账号
    QString inputAccount = accountLine->text();

    // 从数据库中获取账号对应的密码
    emit needPassword(inputAccount);
    // 如果数据库中存在该账号，则设置密码到密码输入框


}
