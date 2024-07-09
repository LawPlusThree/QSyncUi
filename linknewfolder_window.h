#ifndef LINKNEWFOLDER_WINDOW_H
#define LINKNEWFOLDER_WINDOW_H

#include <QWidget>
#include "ElaWidget.h"

class ElaPushButton;
class ElaCheckBox;
class ElaComboBox;
class linkNewFolder_window : public ElaWidget
{
public:
    linkNewFolder_window(QWidget *parent = nullptr);
    ~linkNewFolder_window();
private:
     ElaPushButton* _pushButton1{nullptr};
     ElaPushButton* _pushButton2{nullptr};
     ElaPushButton* _pushButton3{nullptr};
     ElaCheckBox* _checkBox{nullptr};
     ElaComboBox* _comboBox{nullptr};
};

#endif // LINKNEWFOLDER_WINDOW_H
