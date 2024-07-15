#ifndef FILEMANGE_VIEW_H
#define FILEMANGE_VIEW_H

#include "ElaScrollPage.h"
#include "ElaProgressBar.h"
#include "linknewfolder_window.h"
#include "setexcludeditems_view.h"
#include "usermanager.h"

class ElaToggleButton;
class ElaPushButton;
class DirCard;
class QVBoxLayout;
class DirCardProxy;
class FileManagePage:public ElaScrollPage
{
    Q_OBJECT
public:
    FileManagePage(QWidget* parent = nullptr,UserManager *um = nullptr);
    ~FileManagePage();
    void addDirCard(QString filename,QString cloudname,quint64 datasize,QString bindtime,int syncStatus,int id);
    void removeDirCard(int id);
    void removeChecked();
    void modifyDirCard(quint64 datasize,QString bindtime,int id);
    void updateComboBoxIndex(int index);
    DirCardProxy*_dircardProxy;
    linkNewFolder_window* linknewfolderwindow=new linkNewFolder_window();
    setExcludedItems_view* setexcludeditemsview=new setExcludedItems_view();
    QWidget* filenameWidget = new QWidget();
private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton1{nullptr};
    ElaPushButton* _pushButton2{nullptr};
    ElaPushButton* _pushButton3{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
    ElaComboBox* _comboBox{nullptr};

signals:
    void deleteTask(int id);
    void pauseTask(int id);
    void resumeTask(int id);
    void setExcludedItems(QVector<QString> items);
    void setThreadNum(int num);
private slots:
    void buttonShowHide();
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
};

#endif // FILEMANGE_VIEW_H
