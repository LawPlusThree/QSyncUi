#include "resetwin.h"

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
#include"ElaText.h"
#include"ElaImageCard.h"

resetwin::resetwin(QWidget* parent):ElaWidget(parent,400,500)
{
    setWindowTitle("珞珈云");
    QWidget*area=new QWidget();
    this->setWindowModality(Qt::ApplicationModal);
    this->setCentralWidget(area);
    this->hide();
    this->setFixedSize(250,600);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);

    ElaText*text=new ElaText("重置密码");
    text->setTextSize(16);
    QHBoxLayout*textArea=new QHBoxLayout();
    textArea->addWidget(text,0,Qt::AlignCenter);

    QImage image;
    ElaImageCard*avatar=new ElaImageCard();
    avatar->setCardImage(image);
    avatar->setFixedSize(150,150);
    QHBoxLayout*avatarArea=new QHBoxLayout();
    avatarArea->addWidget(avatar,0,Qt::AlignCenter);

    passwordLine=new ElaLineEdit();
    passwordLine->setFixedSize(270,30);
    passwordLine->setPlaceholderText("密码");
    passwordLine->setEchoMode(QLineEdit::Password);
    QHBoxLayout*passwordArea=new QHBoxLayout();
    passwordArea->addWidget(passwordLine,0,Qt::AlignCenter);

    passagainLine=new ElaLineEdit();
    passagainLine->setFixedSize(270,30);
    passagainLine->setPlaceholderText("再次确认密码");
    passagainLine->setEchoMode(QLineEdit::Password);
    QHBoxLayout*passagainArea=new QHBoxLayout();
    passagainArea->addWidget(passagainLine,0,Qt::AlignCenter);

    ElaPushButton*resetBtn=new ElaPushButton("重置");
    resetBtn->setFixedSize(270,30);
    resetBtn->setStyleSheet("background-color:rgb(0,204,255)");
    QHBoxLayout*resetBtnArea=new QHBoxLayout();
    resetBtnArea->addWidget(resetBtn,0,Qt::AlignCenter);

    QVBoxLayout*resetWinArea=new QVBoxLayout(area);
    resetWinArea->addLayout(textArea,Qt::AlignCenter);
    resetWinArea->addLayout(avatarArea,Qt::AlignCenter);
    resetWinArea->addLayout(passwordArea,Qt::AlignCenter);
    resetWinArea->addLayout(passagainArea,Qt::AlignCenter);
    resetWinArea->addLayout(resetBtnArea,Qt::AlignCenter);
    area->setLayout(resetWinArea);

    connect(resetBtn,&ElaPushButton::clicked,this, &resetwin::on_resetBtn_clicked);
}

void resetwin::on_resetBtn_clicked()
{
    if(passwordLine->text()=="")
    {
        QMessageBox::information(this, "错误","请输入新密码！");
    }
    else if(passagainLine->text()=="")
    {
        QMessageBox::information(this, "错误","请再次确认密码！");
    }
    else if(passwordLine->text()!=passagainLine->text())
    {
        QMessageBox::information(this, "错误","两次输入的密码不一致！");
    }
    else
    {
        QMessageBox::information(this, "成功","重置成功");
        emit goback();
    }
}

resetwin::~resetwin()
{
}

void resetwin::closeEvent(QCloseEvent *event)
{
   emit goback();
}
