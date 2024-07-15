#ifndef SETEXCLUDEDITEMS_VIEW_H
#define SETEXCLUDEDITEMS_VIEW_H

#include <QWidget>
#include <QTextEdit>
#include "ElaWidget.h"
#include "usermanager.h"

class ElaPushButton;
class setExcludedItems_view : public ElaWidget
{
public:
    setExcludedItems_view(QWidget *parent = nullptr,UserManager *um=nullptr);
    ~setExcludedItems_view();
    void updateExcludedItems(QString items);

    QTextEdit* textEdit = new QTextEdit(this);
private:
    ElaPushButton* _pushButton2{nullptr};
    ElaPushButton* _pushButton3{nullptr};
signals:
    //void settingExcludedItems(QString);
};

#endif // SETEXCLUDEDITEMS_VIEW_H
