#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ElaContentDialog.h"
#include "ElaWindow.h"
#include"loginwin.h"
#include"ElaCheckBox.h"

#include "user.h"
class HomePage;
class SyncingPage;
class FileManagePage;
class HistorysyncPage;
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
    loginwin* login=new loginwin();
    Q_SLOT void onCloseButtonClicked();
    User*CurrentUser=nullptr;
    DatabaseManager*db;

public slots:
    void onUserLoggedIn(User user);


private:
    HomePage *_homePage{nullptr};
    SyncingPage *_syncingPage{nullptr};
    FileManagePage*_filemanagePage{nullptr};
    HistorysyncPage *_historysyncPage{nullptr};
    T_ElaScreen *_elaScreenPage{nullptr};
    T_Icon *_iconPage{nullptr};
    T_BaseComponents *_baseComponentsPage{nullptr};
    T_TabWidget *_tabWidgetPage{nullptr};
    QString _elaDxgiKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};

    ElaCheckBox* _checkBox{nullptr};

};
#endif // MAINWINDOW_H
