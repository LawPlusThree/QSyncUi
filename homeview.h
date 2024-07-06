#ifndef T_HOME_H
#define T_HOME_H

#include"ElaCheckBox.h"
#include "ElaScrollPage.h"
class ElaMenu;
class HomePage : public ElaScrollPage
{
    Q_OBJECT
public:
    explicit HomePage(QWidget* parent = nullptr);
    ~HomePage();
Q_SIGNALS:
    Q_SIGNAL void elaScreenNavigation();
    Q_SIGNAL void elaBaseComponentNavigation();
    Q_SIGNAL void elaSceneNavigation();
    Q_SIGNAL void elaIconNavigation();

protected:
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    ElaMenu* _homeMenu{nullptr};
    ElaCheckBox* _checkBox{nullptr};
};

#endif // T_HOME_H
