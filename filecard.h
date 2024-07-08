#ifndef FILECARD_H
#define FILECARD_H

#include"ElaScrollPageArea.h"
#include"ElaText.h"

class ElaCheckBox;
class ElaIconButton;
class ElaProgressBar;
class FileCard : public ElaScrollPageArea
{
public:
    explicit FileCard(QString f, QString d,QString s,QString p);

    ElaCheckBox* _checkBox{nullptr};
    ElaText *filename;
    ElaText *datasize;
    ElaText *speed;
    ElaText*progress;
    ElaIconButton*modifyBtn;
    ElaIconButton*pauseBtn;
    ElaIconButton*relieveBtn;
    ElaProgressBar*proBar;
};

#endif // FILECARD_H
