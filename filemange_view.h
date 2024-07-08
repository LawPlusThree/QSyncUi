#ifndef FILEMANGE_VIEW_H
#define FILEMANGE_VIEW_H

#include "ElaScrollPage.h"
#include "ElaProgressBar.h"

class ElaToggleButton;
class ElaPushButton;
class DirCard;
class QVBoxLayout;
class FileManagePage:public ElaScrollPage
{
public:
    FileManagePage(QWidget* parent = nullptr);
    void addDirCard(DirCard*newDirCard);
    QVBoxLayout*filesArea;

private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton1{nullptr};
    ElaPushButton* _pushButton2{nullptr};
    ElaPushButton* _pushButton3{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
};

#endif // FILEMANGE_VIEW_H
