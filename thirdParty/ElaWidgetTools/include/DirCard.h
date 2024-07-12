#ifndef DIRCARD_H
#define DIRCARD_H

#include <QPushButton>
#include"ElaScrollPageArea.h"
#include"ElaText.h"
#include"ElaIconButton.h"

#include "stdafx.h"
class ElaCheckBox;
class DirCardPrivate;
class ElaToggleButton;
class ElaPushButton;
class ELA_EXPORT DirCard : public ElaScrollPageArea
{
    Q_OBJECT
public:
    explicit DirCard(QString f, quint64 d,QString b,int Id);
    ElaCheckBox* getCheckBox() const;

    ElaCheckBox* _checkBox{nullptr};
    ElaIconButton*pauseBtn;
    ElaText *filename;
    ElaText *datasize;
    ElaText *bindtime;
    ElaIconButton*relieveBtn;
    void modify(quint64 d,QString b);
    QString fullText;

    int id;
    bool ischecked();

private slots:
    void on_relieveBtn_clicked();
    void on_pauseBtn_clicked();
signals:
    void relieve(int id);

    friend class DirCardProxy;
};

#endif // ELAREMINDERCARD_H
