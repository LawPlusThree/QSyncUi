#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include "ElaScrollPage.h"
#include"ElaProgressBar.h"

class ElaToggleButton;
class ElaPushButton;
class DirCard;
class QVBoxLayout;
class HistoryViewPage : public ElaScrollPage
{
public:
    explicit HistoryViewPage(QWidget* parent);
    QVBoxLayout*filesLayout;
    void addHistoryViewCard(QString filename,QString datasize,QString bingtime);
    QWidget* filenameWidget = new QWidget();

private:
    ElaProgressBar* _progressBar{nullptr};
};

#endif // HISTORYVIEW_H
