#ifndef SYNCING_VIEW_H
#define SYNCING_VIEW_H

#include "ElaProgressBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPage.h"

class ElaToggleButton;
class ElaPushButton;
class QVBoxLayout;
class FileCardProxy;
class SyncingPage : public ElaScrollPage
{
public:
    SyncingPage(QWidget* parent = nullptr);
    ~SyncingPage();
    void addFile(QString filename, int datasize,int speed,int progress,int id);
    void removeFile(int id);
    void modifyFile(int d,int s,int p,int id);
    void modifyFile(int p,int id);
    void totalProgress();
    FileCardProxy* _filecardProxy;
private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
};

#endif // SYNCING_VIEW_H
