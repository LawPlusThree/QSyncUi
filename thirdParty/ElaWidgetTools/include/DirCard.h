#ifndef DIRCARD_H
#define DIRCARD_H

#include <QPushButton>
#include"ElaScrollPageArea.h"

#include "stdafx.h"
class ElaCheckBox;
class DirCardPrivate;
class ELA_EXPORT DirCard : public ElaScrollPageArea
{
public:
    explicit DirCard(QString filename,QString datasize,QString bingtime);
    ~DirCard();
private:
    ElaCheckBox* _checkBox{nullptr};
    QString filename;
    QString datasize;
    QString bingtime;
};

#endif // ELAREMINDERCARD_H
