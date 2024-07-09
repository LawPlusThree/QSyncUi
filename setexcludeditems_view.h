#ifndef SETEXCLUDEDITEMS_VIEW_H
#define SETEXCLUDEDITEMS_VIEW_H

#include <QWidget>
#include "ElaWidget.h"

class ElaPushButton;
class setExcludedItems_view : public ElaWidget
{
public:
    setExcludedItems_view(QWidget *parent = nullptr);
    ~setExcludedItems_view();
private:
    ElaPushButton* _pushButton2{nullptr};
    ElaPushButton* _pushButton3{nullptr};
signals:
};

#endif // SETEXCLUDEDITEMS_VIEW_H
