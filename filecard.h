#ifndef FILECARD_H
#define FILECARD_H

#include"ElaScrollPageArea.h"
#include"ElaText.h"
#include<QDateTime>

class ElaCheckBox;
class ElaIconButton;
class ElaProgressBar;
class FileCard : public ElaScrollPageArea
{
    Q_OBJECT
public:
    //explicit FileCard(QString f, QString d,QString s,QString p);
    explicit FileCard(QString f, int d,int s,int p,int Id);
    //void modify(int d,int s,int p);
    void modify(int totalsize,int currentsize);
    void processing(int p);

    ElaCheckBox* _checkBox{nullptr};
    ElaText *filename;
    ElaText *datasize;
    ElaText *speed;
    //ElaText*progress;
    ElaIconButton*modifyBtn;
    ElaIconButton*pauseBtn;
    ElaIconButton*relieveBtn;
    ElaProgressBar*proBar;
    int id;
    int progress;

    QDateTime preTime;
    QDateTime currentTime;
    int preSize;
    int Size;

private slots:
    void on_relieveBtn_clicked();
    void on_pauseBtn_clicked();
signals:
    void Relieve(int id);
};

#endif // FILECARD_H
