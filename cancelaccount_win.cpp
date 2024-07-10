#include "cancelaccount_win.h"

cancelaccount_win::cancelaccount_win(QWidget *parent)
    : ElaWidget(parent,400,500)
{
    this->setWindowModality(Qt::ApplicationModal);
    this->hide();

    //this->setCentralWidget(new QWidget());
}

cancelaccount_win::~cancelaccount_win()
{
}
