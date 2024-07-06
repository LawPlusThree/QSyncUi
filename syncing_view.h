#ifndef SYNCING_VIEW_H
#define SYNCING_VIEW_H

#include "ElaProgressBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPageArea.h"
class SyncingPage : public ElaScrollPage
{
public:
    SyncingPage(QWidget* parent = nullptr);
    ~SyncingPage();
private:
    ElaProgressBar* _progressBar{nullptr};
};

#endif // SYNCING_VIEW_H
