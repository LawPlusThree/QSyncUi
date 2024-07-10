#ifndef DIRCARD_H
#define DIRCARD_H

#include <QPushButton>
#include"ElaScrollPageArea.h"
#include"ElaText.h"

#include "stdafx.h"
class ElaCheckBox;
class DirCardPrivate;
class ElaToggleButton;
class ElaPushButton;
class ELA_EXPORT DirCard : public ElaScrollPageArea
{
    Q_OBJECT
public:
    explicit DirCard(QString f, QString d,QString b);

    ElaCheckBox* _checkBox{nullptr};
    ElaToggleButton*pauseBtn;
    ElaText *filename;
    ElaText *datasize;
    ElaText *bindtime;
    ElaPushButton*relieveBtn;

    QString id;
    bool ischecked();

private slots:
    void on_relieveBtn_clicked();
signals:
    void relieve(QString id);

    friend class DirCardProxy;
};

#endif // ELAREMINDERCARD_H
