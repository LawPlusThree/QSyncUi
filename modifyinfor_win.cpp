#include "modifyinfor_win.h"

modifyInfor_win::modifyInfor_win(QWidget *parent)
    : ElaWidget(parent,400,500)
{
    this->setWindowModality(Qt::ApplicationModal);
    this->hide();

    //this->setCentralWidget(new QWidget());
}

modifyInfor_win::~modifyInfor_win()
{
}
