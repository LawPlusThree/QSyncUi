#ifndef SYNCING_VIEW_H
#define SYNCING_VIEW_H

#include "ElaProgressBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPage.h"

class ElaToggleButton;
class ElaPushButton;
class QVBoxLayout;
class SyncingPage : public ElaScrollPage
{
public:
    SyncingPage(QWidget* parent = nullptr);
    ~SyncingPage();
    void addFile(QString filename, QString datasize,QString speed,QString progress);
    QVBoxLayout*filesLayout;
private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
};

#endif // SYNCING_VIEW_H
