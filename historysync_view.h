#ifndef HISTORYSYNC_VIEW_H
#define HISTORYSYNC_VIEW_H

#include "ElaProgressBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPageArea.h"

class HistorysyncPage : public ElaScrollPage
{
public:
    HistorysyncPage(QWidget* parent = nullptr);
    ~HistorysyncPage();
private:
    ElaProgressBar* _progressBar{nullptr};
};

#endif // HISTORYSYNC_VIEW_H
