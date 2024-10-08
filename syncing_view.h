#ifndef SYNCING_VIEW_H
#define SYNCING_VIEW_H

#include<QDateTime>
#include "ElaProgressBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPage.h"
#include "filecard.h"

class ElaToggleButton;
class ElaPushButton;
class QVBoxLayout;
class FileCardProxy;
class SyncingPage : public ElaScrollPage
{
    Q_OBJECT
public:
    SyncingPage(QWidget* parent = nullptr);
    ~SyncingPage();
    void addFile(QString filename, quint64 datasize,double speed,int progress,int syncStatus,int id);
    void removeFile(int id);
    //void modifyFile(int d,int s,int p,int id);
    //void modifyFile(int p,int id);
    void modifyFile(quint64 totalSize,quint64 currentSize,int id);
    void totalProgress();
    void PauseChecked();
    void CancelChecked();
    FileCardProxy* _filecardProxy;
    QDateTime preTime;
    QDateTime currentTime;
    int preSize=0;
    int Size=0;
    QWidget* filenameWidget = new QWidget();
private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
    ElaPushButton* _PauseButton{nullptr};
    ElaPushButton* _CancelButton{nullptr};
    ElaPushButton*_selectAllButton{nullptr};
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void toggleSelectAll();
    void buttonShowHide();
};

#endif // SYNCING_VIEW_H
