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
    explicit FileCard(QString filename, quint64 datasize,double speed,int progress,int syncStatus,int Id);
    //void modify(int d,int s,int p);
    void modify(quint64 totalsize,quint64 currentsize);
    void processing(int p);

    ElaCheckBox* _checkBox{nullptr};
    ElaText *filename;
    ElaText *datasize;
    ElaText *speed;
    //ElaText*progress;
    ElaIconButton*modifyBtn;
    ElaIconButton*pauseBtn;
    ElaIconButton*relieveBtn;
    ElaIconButton*syncBtn;
    ElaProgressBar*proBar;
    QString fullText;
    int id;
    int progress;

    QDateTime preTime;
    QDateTime currentTime;
    quint64 preSize;
    quint64 Size;

private slots:
    void on_relieveBtn_clicked();
    void on_pauseBtn_clicked();
signals:
    void Relieve(int id);
};

#endif // FILECARD_H
