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
    bool eventFilter(QObject *watched, QEvent *event);

    ElaCheckBox* _checkBox{nullptr};
    ElaText *filename;
    ElaText *datasize;
    ElaText *time;
    bool upif;
    QString fullText;
};

#endif // HISTORYCARD_H
