#ifndef HISTORYVIEWCARD_H
#define HISTORYVIEWCARD_H
#include"ElaScrollPageArea.h"
#include"ElaText.h"
#include"ElaPushButton.h"

class ElaCheckBox;
class ElaProgressBar;
class HistoryViewCard : public ElaScrollPageArea
{
public:
    explicit HistoryViewCard(QString f, QString d,QString t);

    ElaCheckBox* _checkBox{nullptr};
    ElaText *filename;
    ElaText *datasize;
    ElaText *time;
    ElaPushButton *rollback;
    QString fullText;
};

#endif // HISTORYVIEWCARD_H
