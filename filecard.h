#ifndef FILECARD_H
#define FILECARD_H

#include"ElaScrollPageArea.h"
#include"ElaText.h"

class ElaCheckBox;
class ElaIconButton;
class ElaProgressBar;
class FileCard : public ElaScrollPageArea
{
    Q_OBJECT
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

private slots:
    void on_relieveBtn_clicked();
    void on_pauseBtn_clicked();
signals:
    void Relieve();
};

#endif // FILECARD_H
