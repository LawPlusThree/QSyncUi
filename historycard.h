#ifndef HISTORYCARD_H
#define HISTORYCARD_H

#include"ElaScrollPageArea.h"
#include"ElaText.h"

class ElaCheckBox;
class ElaIconButton;
class ElaProgressBar;
class HistoryCard : public ElaScrollPageArea
{
public:
    explicit HistoryCard(QString f, QString d,QString t,bool u);

    ElaCheckBox* _checkBox{nullptr};
    ElaText *filename;
    ElaText *datasize;
    ElaText *time;
    bool upif;
};

#endif // HISTORYCARD_H
