#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ElaContentDialog.h"
#include "ElaWindow.h"
#include"loginwindow.h"
class HomePage;
class SyncingPage;
class T_Icon;
class T_ElaScreen;
class T_BaseComponents;
class T_TabWidget;
class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    LoginWindow*login=new LoginWindow();
    Q_SLOT void onCloseButtonClicked();


private:
    HomePage *_homePage{nullptr};
    SyncingPage *_syncingPage{nullptr};
    T_ElaScreen *_elaScreenPage{nullptr};
    T_Icon *_iconPage{nullptr};
    T_BaseComponents *_baseComponentsPage{nullptr};
    T_TabWidget *_tabWidgetPage{nullptr};
    QString _elaDxgiKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};
};
#endif // MAINWINDOW_H
