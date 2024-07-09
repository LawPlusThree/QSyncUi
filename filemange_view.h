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
class FileManagePage:public ElaScrollPage
{
public:
    FileManagePage(QWidget* parent = nullptr);
    ~FileManagePage();
    void addDirCard(QString filename,QString datasize,QString bingtime);
    void removeDirCard();
    QVBoxLayout*filesLayout;

private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton1{nullptr};
    ElaPushButton* _pushButton2{nullptr};
    ElaPushButton* _pushButton3{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
    linkNewFolder_window* linknewfolderwindow=new linkNewFolder_window();
    setExcludedItems_view* setexcludeditemsview=new setExcludedItems_view();
};

#endif // FILEMANGE_VIEW_H
