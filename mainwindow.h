#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ElaContentDialog.h"
#include "ElaWindow.h"
#include"loginwin.h"
#include"ElaCheckBox.h"
#include"usermanager.h"
#include "user.h"
#include <QQueue>
#include "synctask.h"
#include "synccore.h"
class HomePage;
class SyncingPage;
class FileManagePage;
class HistorysyncPage;
class T_Icon;
class T_ElaScreen;
class T_BaseComponents;
class T_TabWidget;
class HistoryViewPage;
class modifyInfor_win;
class cancelaccount_win;
class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    loginwin* login=new loginwin();
    Q_SLOT void onCloseButtonClicked();
    User*CurrentUser=nullptr;
    UserManager *um;
public slots:
    void onUserLoggedIn(User user);
    void onNeedPassword(const QString& account);
    //void insertUserToDatabase(User user);
    void onMessage( QString message,QString type);
    void onUserAddNewTask(const SyncTask &task);
    void onModifyInfo(User user);
    void onUserdelete();
    void exitLogin();
    void onFileUploadTaskCreated(const QString &localPath, int fileTaskId);
    void onFileUploadTaskUpdated(int fileTaskId, qint64 nowSize, qint64 totalSize);
    void onFileUploadTaskPaused(int fileTaskId);
    void onFileDownloadTaskCreated(const QString &localPath, int fileTaskId);
    void onFileDownloadTaskUpdated(int fileTaskId, qint64 nowSize, qint64 totalSize);
    void onFileDownloadTaskPaused(int fileTaskId);
    void onTaskTotalSize(qint64 size, int taskid);
    void onTaskUploadSize(qint64 size, int taskid);
    void onFileUploadTaskFinished(int fileTaskId);
    void onFileDownloadTaskFinished(int fileTaskId);
    void onUserPausedFileTask(int fileTaskId);

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
    modifyInfor_win* _modifyInfor_win{nullptr};
    SyncCore* _syncCore{nullptr};
    SyncTaskDatabaseManager* _syncTaskDatabaseManager{nullptr};
    void autologin();

};

#endif // MAINWINDOW_H
