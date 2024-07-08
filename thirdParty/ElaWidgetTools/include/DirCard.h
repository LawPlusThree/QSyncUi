#ifndef DIRCARD_H
#define DIRCARD_H

#include <QPushButton>
#include"ElaScrollPageArea.h"
#include"ElaText.h"

#include "stdafx.h"
class ElaCheckBox;
class DirCardPrivate;
class ElaToggleButton;
class ELA_EXPORT DirCard : public ElaScrollPageArea
{
public:
    explicit DirCard(QString f, QString d,QString b);
    ~DirCard();

    ElaCheckBox* _checkBox{nullptr};
    ElaToggleButton*pauseBtn;
    ElaText *filename;
    ElaText *datasize;
    ElaText *bindtime;
};

#endif // ELAREMINDERCARD_H
