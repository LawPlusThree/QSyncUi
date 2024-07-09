#ifndef HISTORYSYNC_VIEW_H
#define HISTORYSYNC_VIEW_H

#include "ElaProgressBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPageArea.h"
#include"ElaPushButton.h"

class QVBoxLayout;
class HistorysyncPage : public ElaScrollPage
{
public:
    HistorysyncPage(QWidget* parent = nullptr);
    ~HistorysyncPage();
    QVBoxLayout*filesLayout;
    void addHistory(QString filename, QString datasize,QString time,bool upif);
private:
    ElaProgressBar* _progressBar{nullptr};
};

#endif // HISTORYSYNC_VIEW_H
