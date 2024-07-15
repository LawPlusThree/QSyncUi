#ifndef HISTORYVIEWCARD_H
#define HISTORYVIEWCARD_H
#include"ElaScrollPageArea.h"
#include"ElaText.h"
#include"ElaPushButton.h"

class ElaCheckBox;
class ElaProgressBar;
class ElaIconButton;
class HistoryViewCard : public ElaScrollPageArea
{
public:
    explicit HistoryViewCard(QString filename,QString cloudname);

    ElaIconButton*button;
    ElaText *filename;
    ElaText*cloudname;
    QString fullText;
    QString cfullText;
};

class SubCard:public ElaScrollPageArea
{
public:
    explicit SubCard(QString versionID,quint64 datasize,QString bindtime);

    ElaText*versionID;
    quint64 datasize;
    ElaText*bindtime;
    ElaPushButton*rollback;

protected:
    void paintEvent(QPaintEvent*event)override;
};

#endif // HISTORYVIEWCARD_H
