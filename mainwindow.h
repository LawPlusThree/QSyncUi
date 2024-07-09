#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ElaContentDialog.h"
#include "ElaWindow.h"
#include"loginwin.h"
#include"ElaCheckBox.h"
#include"userinfopage.h"

#include "user.h"
class HomePage;
class SyncingPage;
class FileManagePage;
class HistorysyncPage;
class T_Icon;
class T_ElaScreen;
class T_BaseComponents;
class T_TabWidget;
class HistoryViewPage;
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
    void onNeedPassword(const QString& account);

signals:
    void dbPassword(const QString &);


private:
    HomePage *_homePage{nullptr};
    SyncingPage *_syncingPage{nullptr};
    FileManagePage*_filemanagePage{nullptr};
    HistorysyncPage *_historysyncPage{nullptr};
    HistoryViewPage *_historyviewPage{nullptr};
    T_ElaScreen *_elaScreenPage{nullptr};
    T_Icon *_iconPage{nullptr};
    T_BaseComponents *_baseComponentsPage{nullptr};
    T_TabWidget *_tabWidgetPage{nullptr};
    QString _elaDxgiKey{""};
    QString _aboutKey{""};
    QString _settingKey{""};

    ElaCheckBox* _checkBox{nullptr};
    UserInfoPage* _userinfopage{nullptr};

};
#endif // MAINWINDOW_H
