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
    void addDirCard(QString filename,quint64 datasize,QString bindtime,int id);
    void removeDirCard(int id);
    void modifyDirCard(quint64 datasize,QString bindtime,int id);
    DirCardProxy*_dircardProxy;
    linkNewFolder_window* linknewfolderwindow=new linkNewFolder_window();
    QWidget* filenameWidget = new QWidget();
private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton1{nullptr};
    ElaPushButton* _pushButton2{nullptr};
    ElaPushButton* _pushButton3{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
    ElaComboBox* _comboBox{nullptr};

    setExcludedItems_view* setexcludeditemsview=new setExcludedItems_view();
signals:
    void deleteTask(int id);
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
};

#endif // FILEMANGE_VIEW_H
