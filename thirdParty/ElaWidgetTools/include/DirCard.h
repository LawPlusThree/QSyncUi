#ifndef DIRCARD_H
#define DIRCARD_H

#include <QPushButton>
#include"ElaScrollPageArea.h"
#include"ElaText.h"

#include "stdafx.h"
class ElaCheckBox;
class DirCardPrivate;
class ELA_EXPORT DirCard : public ElaScrollPageArea
{
public:
    explicit DirCard(QString f, QString d,QString b);
    ~DirCard();
private:
    ElaCheckBox* _checkBox{nullptr};
    ElaText *filename;
    ElaText *datasize;
    ElaText *bindtime;
};

#endif // ELAREMINDERCARD_H
