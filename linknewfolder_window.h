#ifndef LINKNEWFOLDER_WINDOW_H
#define LINKNEWFOLDER_WINDOW_H

#include <QWidget>
#include "ElaWidget.h"
#include "synctask.h"
class ElaPushButton;
class ElaCheckBox;
class ElaComboBox;
class linkNewFolder_window : public ElaWidget
{
    Q_OBJECT
public:
    linkNewFolder_window(QWidget *parent = nullptr);
    ~linkNewFolder_window();
signals:
    void onNewTask(const SyncTask &task);
private:
     ElaPushButton* _pushButton1{nullptr};
     ElaPushButton* _pushButton2{nullptr};
     ElaPushButton* _pushButton3{nullptr};
     ElaCheckBox* _checkBox{nullptr};
     ElaComboBox* _comboBox{nullptr};
};

#endif // LINKNEWFOLDER_WINDOW_H
