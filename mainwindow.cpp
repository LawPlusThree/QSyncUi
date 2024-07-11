#include "mainwindow.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "ElaContentDialog.h"
#include "ElaEventBus.h"
#include "ElaGraphicsItem.h"
#include "ElaGraphicsScene.h"
#include "ElaGraphicsView.h"
#include "ElaMessageBar.h"
#include "ElaWidget.h"
#include "homeView.h"
#include "linknewfolder_window.h"
#include "DirCard.h"
#include "dircardproxy.h"
#include "loginwin.h"
#include "syncing_view.h"
#include"filemange_view.h"
#include "historysync_view.h"
#include"historyview.h"
#include "SyncThread.h"
#include "qthread.h"
#include "modifyinfor_win.h"

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
{

    db = new DatabaseManager(this); // 创建数据库管理器实例
    db->initializeDatabase(); // 初始化数据库
    setUserInfoCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    setUserInfoCardTitle("未登录");
    setUserInfoCardSubTitle("");
    setWindowTitle("珞珈云");
    _syncingPage = new SyncingPage(this);
    _filemanagePage=new FileManagePage(this);
    _historysyncPage = new HistorysyncPage(this);
    _historyviewPage = new HistoryViewPage(this);

    _filemanagePage->modifyDirCard(119,"2024.8.20",6);

    connect(this, &ElaWindow::userInfoCardClicked, [=]() {
        if(CurrentUser==nullptr)
            login->show();
    });
    connect(this,&MainWindow::dbPassword,login,&loginwin::on_db_response);
    connect(login,&loginwin::needPassword,this,&MainWindow::onNeedPassword);
    connect(login->signinWin, &signinwin::on_signin_complete, this, &MainWindow::insertUserToDatabase);
    qDebug()<<connect(login->channel,&MessageChannel::message,this,&MainWindow::onMessage);
    connect(_filemanagePage->linknewfolderwindow,&linkNewFolder_window::onNewTask,this,&MainWindow::onUserAddNewTask);
    ElaGraphicsScene *scene = new ElaGraphicsScene(this);
    scene->setSceneRect(0, 0, 1500, 1500);
    ElaGraphicsItem *item1 = new ElaGraphicsItem();
    item1->setWidth(100);
    item1->setHeight(100);
    ElaGraphicsItem *item2 = new ElaGraphicsItem();
    item2->setWidth(100);
    item2->setHeight(100);

    scene->addItem(item1);
    scene->addItem(item2);
    ElaGraphicsView *view = new ElaGraphicsView(scene);
    view->setScene(scene);

    QString testKey_1;
    QString testKey_2;
    QString testKey_3;
    QString testKey_4;
    QString modifyKey;
    QString cancelKey;
    QString logoutKey;

    addExpanderNode("同步功能",testKey_2,ElaIconType::House);
    addPageNode("正在同步",_syncingPage,testKey_2,ElaIconType::Cloud);
    addPageNode("历史同步",_historysyncPage,testKey_2,ElaIconType::CheckToSlot);
    addPageNode("同步文件夹管理",_filemanagePage,testKey_2,ElaIconType::FolderClosed);
    addExpanderNode("版本控制",testKey_3,ElaIconType::EnvelopeOpenText);
    addPageNode("查看历史",_historyviewPage,testKey_3,ElaIconType::CalendarClock);

    addFooterNode("修改信息", nullptr, modifyKey, 0, ElaIconType::Text);
    addFooterNode("注销账号", nullptr, cancelKey, 0, ElaIconType::UserSlash);
    addFooterNode("退出登录", nullptr, logoutKey, 0, ElaIconType::ArrowRightFromBracket);
    _modifyInfor_win=new modifyInfor_win();
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)
            {
                if (modifyKey == nodeKey)
                {
                    _modifyInfor_win->show();
                }
                else if(cancelKey==nodeKey)
                {
                    QWidget* _centralWidget = new QWidget(this);
                    QVBoxLayout* centralVLayout = new QVBoxLayout(_centralWidget);
                    centralVLayout->setContentsMargins(9, 15, 9, 20);
                    ElaText* title = new ElaText("注销账号", this);
                    title->setTextStyle(ElaTextType::Title);
                    ElaText* subTitle = new ElaText("确定要注销账号吗", this);
                    subTitle->setTextStyle(ElaTextType::Body);
                    centralVLayout->addWidget(title);
                    centralVLayout->addWidget(subTitle);
                    centralVLayout->addStretch();
                    ElaContentDialog *dialag = new ElaContentDialog(this,false);
                    dialag->setCentralWidget(_centralWidget);
                    dialag->setLeftButtonText("取消");
                    dialag->setRightButtonText("确认");
                    //connect(dialag, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
                    dialag->show();
                }
                else if(logoutKey==nodeKey)
                {
                    QWidget* _logoutWidget = new QWidget(this);
                    QVBoxLayout* logoutVLayout = new QVBoxLayout(_logoutWidget);
                    logoutVLayout->setContentsMargins(9, 15, 9, 20);
                    ElaText* logoutTitle = new ElaText("退出登录", this);
                    logoutTitle->setTextStyle(ElaTextType::Title);
                    ElaText* logoutSubTitle = new ElaText("确定要退出登录吗", this);
                    logoutSubTitle->setTextStyle(ElaTextType::Body);
                    logoutVLayout->addWidget(logoutTitle);
                    logoutVLayout->addWidget(logoutSubTitle);
                    logoutVLayout->addStretch();
                    ElaContentDialog *logoutdialag = new ElaContentDialog(this,false);
                    logoutdialag->setCentralWidget(_logoutWidget);
                    logoutdialag->setLeftButtonText("取消");
                    logoutdialag->setRightButtonText("确认");
                    connect(logoutdialag, &ElaContentDialog::rightButtonClicked, this, &MainWindow::exitLogin);
                    logoutdialag->show();
                }
    });

    qDebug() << ElaEventBus::getInstance()->getRegisteredEventsName();
    QObject::connect(login, &loginwin::on_login_complete, this, &MainWindow::onUserLoggedIn);
    QObject::connect(_modifyInfor_win, &modifyInfor_win::changexinxi, this, &MainWindow::onModifyInfo);
    // 拦截默认关闭事件
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, &MainWindow::onCloseButtonClicked);
}

MainWindow::~MainWindow() {}
void MainWindow::onUserLoggedIn(User user)
{
    CurrentUser=new User(user);
    connect(CurrentUser->channel,&MessageChannel::message,this,&MainWindow::onMessage);
    _modifyInfor_win->currentUser=CurrentUser;
    //db->insertUser(user.getEmail(),user.gethashedPassword());
    setUserInfoCardTitle(user.getUsername());
    setUserInfoCardSubTitle(user.getEmail());
    _syncCore=new SyncCore(this);
    _syncTaskDatabaseManager=new SyncTaskDatabaseManager(CurrentUser);
    QString url=user.avatarpath;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QFile file("downloaded_image.jpg");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.close();
    }
    delete reply;
    QString filename=QDir::toNativeSeparators(file.fileName());
    QPixmap pix(filename);
    setUserInfoCardPixmap(pix);
    for (auto const &x:_syncTaskDatabaseManager->getTasks()){
        if (x.getLastSyncTime()==QDateTime::fromString("2000-01-01 00:00:00","yyyy-MM-dd hh:mm:ss"))
        {
            QString timeDelta="从未同步";
            this->_filemanagePage->addDirCard(x.getLocalPath(),11,timeDelta,x.getId());
            continue;
        }else{
            QString timeDelta=QString::number(x.getLastSyncTime().daysTo(QDateTime::currentDateTime()))+"天前";
            this->_filemanagePage->addDirCard(x.getLocalPath(),11,timeDelta,x.getId());
        }
    }
}

void MainWindow::exitLogin()
{
    setUserInfoCardTitle("未登录");
    setUserInfoCardSubTitle("");
    setUserInfoCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    CurrentUser=nullptr;
}

void MainWindow::onNeedPassword(const QString &account)
{
    QString password = db->getUserPassword(account).second;
    emit dbPassword(password);
}

void MainWindow::insertUserToDatabase(User user)
{
    qDebug()<<user.getEmail()<<" "<<user.gethashedPassword();
    db->insertUser(user.getEmail(),user.gethashedPassword());
    qDebug()<<db->getUserPassword(user.getEmail());
}


void MainWindow::onMessage( QString message, QString type)
{
    /* static void success(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void warning(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void information(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);
    static void error(ElaMessageBarType::PositionPolicy policy, QString title, QString text, int displayMsec, QWidget* parent = nullptr);*/
    QString title;
    if (type=="Info")
    {
        ElaMessageBar::information( ElaMessageBarType::TopRight,QString("消息"),message,2000,this);
    }
    else if (type=="Error")
    {
        ElaMessageBar::error(ElaMessageBarType::TopRight,QString("错误"),message,2000,this);
    }
    else if (type=="Warning")
    {
        ElaMessageBar::warning(ElaMessageBarType::TopRight,QString("警告"),message,2000,this);
    }
    else if (type=="Success")
    {
        ElaMessageBar::success(ElaMessageBarType::TopRight,QString("成功"),message,2000,this);
    }
}

void MainWindow::onUserAddNewTask(const SyncTask &task)
{
    if(CurrentUser==nullptr)
    {
        onMessage("请先登录","Error");
        return;
    }
    if(
        CurrentUser->addTask(task.getLocalPath(),task.getRemotePath(),task.getSyncStatus(),1,1)){
        if(_syncCore!=nullptr)
        {
            SyncTask mytask(task);
            _syncCore->addTask(&mytask);
        }
        if(_syncTaskDatabaseManager!=nullptr)
        {
            _syncTaskDatabaseManager->addTask(task);
        }
        if (task.getLastSyncTime()==QDateTime::fromString("2000-01-01 00:00:00","yyyy-MM-dd hh:mm:ss"))
        {
            QString timeDelta="从未同步";
            this->_filemanagePage->addDirCard(task.getLocalPath(),111,timeDelta,task.getId());
        }else{
            QString timeDelta=QString::number(task.getLastSyncTime().daysTo(QDateTime::currentDateTime()))+"天前";
            this->_filemanagePage->addDirCard(task.getLocalPath(),1111,timeDelta,task.getId());
        }
    }
}

void MainWindow::onCloseButtonClicked()
{
    ElaContentDialog *dialag = new ElaContentDialog(this);
    dialag->setLeftButtonText("取消");
    dialag->setMiddleButtonText("最小化");
    dialag->setRightButtonText("退出");
    connect(dialag, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(dialag, &ElaContentDialog::middleButtonClicked, this, &MainWindow::showMinimized);
    dialag->show();
}

void MainWindow::onModifyInfo(User user)
{
    //CurrentUser=new User(user);
    _modifyInfor_win->currentUser=CurrentUser;
    qDebug() << user.getEmail() << " " << user.gethashedPassword();
    db->updateUserInfo(user.getEmail(),user.gethashedPassword());
    qDebug() << user.getEmail() << " " << user.gethashedPassword() <<" " << db->getUserPassword(user.getEmail());
    setUserInfoCardTitle(_modifyInfor_win->newIdEdit_->text());
    setUserInfoCardSubTitle(user.getEmail());

    QString url=user.avatarpath;
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    QNetworkReply *reply = manager->get(request);
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();
    QFile file("downloaded_image.jpg");
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.close();
    }
    delete reply;
    QString filename=QDir::toNativeSeparators(file.fileName());
    QPixmap pix(filename);
    setUserInfoCardPixmap(pix);
}

void MainWindow::onFileUploadTaskCreated(const QString &localPath, qint64 size, int fileTaskId)
{
    // onFileUploadTaskCreated implementation
}

void MainWindow::onFileUploadTaskUpdated(const QString &localPath, qint64 nowSize, int fileTaskId)
{
    // onFileUploadTaskUpdated implementation
}

void MainWindow::onFileUploadTaskPaused(int fileTaskId)
{
    // onFileUploadTaskPaused implementation
}

void MainWindow::onFileDownloadTaskCreated(const QString &localPath, qint64 size, int fileTaskId)
{
    // onFileDownloadTaskCreated implementation
}

void MainWindow::onFileDownloadTaskUpdated(const QString &localPath, qint64 nowSize, int fileTaskId)
{
    // onFileDownloadTaskUpdated implementation
}

void MainWindow::onFileDownloadTaskPaused(int fileTaskId)
{
    // onFileDownloadTaskPaused implementation
}

void MainWindow::onTaskTotalSize(qint64 size, int taskid)
{
    // onTaskTotalSize implementation
}

void MainWindow::onTaskUploadSize(qint64 size, int taskid)
    {
        // onTaskUploadSize implementation
    };
