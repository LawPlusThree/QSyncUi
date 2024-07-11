#ifndef FILEMANGE_VIEW_H
#define FILEMANGE_VIEW_H

#include "ElaScrollPage.h"
#include "ElaProgressBar.h"
#include "linknewfolder_window.h"
#include "setexcludeditems_view.h"

class ElaToggleButton;
class ElaPushButton;
class DirCard;
class QVBoxLayout;
class DirCardProxy;
class FileManagePage:public ElaScrollPage
{
    Q_OBJECT
public:
    FileManagePage(QWidget* parent = nullptr);
    ~FileManagePage();
    void addDirCard(QString filename,int datasize,QString bindtime,int id);
    void removeDirCard(int id);
    void modifyDirCard(int datasize,QString bindtime,int id);
    DirCardProxy*_dircardProxy;
    linkNewFolder_window* linknewfolderwindow=new linkNewFolder_window();
    QWidget* filenameWidget = new QWidget();
private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton1{nullptr};
    ElaPushButton* _pushButton2{nullptr};
    ElaPushButton* _pushButton3{nullptr};
    ElaToggleButton* _toggleButton{nullptr};

    setExcludedItems_view* setexcludeditemsview=new setExcludedItems_view();
signals:
    void deleteTask(int id);
};

#endif // FILEMANGE_VIEW_H
