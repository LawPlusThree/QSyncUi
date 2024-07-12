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

    um = new UserManager(this);
    setUserInfoCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    setUserInfoCardTitle("未登录");
    setUserInfoCardSubTitle("");
    setWindowTitle("珞珈云");
    _syncingPage = new SyncingPage(this);
    _filemanagePage=new FileManagePage(this);
    _historysyncPage = new HistorysyncPage(this);
    _historyviewPage = new HistoryViewPage(this);

    connect(this, &ElaWindow::userInfoCardClicked, [=]() {
        if(CurrentUser==nullptr)
            login->show();
        else{
            onMessage("若要登录其他账号，请先退出当前账号！","Info");
        }
    });
    connect(this,&MainWindow::dbPassword,login,&loginwin::on_db_response);
    connect(login,&loginwin::needPassword,this,&MainWindow::onNeedPassword);
    //connect(login->signinWin, &signinwin::on_signin_complete, this, &MainWindow::insertUserToDatabase);
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
    QString feedbackKey;

    addExpanderNode("同步功能",testKey_2,ElaIconType::House);
    addPageNode("正在同步",_syncingPage,testKey_2,ElaIconType::Cloud);
    addPageNode("历史同步",_historysyncPage,testKey_2,ElaIconType::CheckToSlot);
    addPageNode("同步文件夹管理",_filemanagePage,testKey_2,ElaIconType::FolderClosed);
    addExpanderNode("版本控制",testKey_3,ElaIconType::EnvelopeOpenText);
    addPageNode("查看历史",_historyviewPage,testKey_3,ElaIconType::CalendarClock);

    addFooterNode("用户反馈", nullptr, feedbackKey, 0, ElaIconType::MessageArrowUp);
    addFooterNode("修改信息", nullptr, modifyKey, 0, ElaIconType::Text);
    addFooterNode("注销账号", nullptr, cancelKey, 0, ElaIconType::UserSlash);
    addFooterNode("退出登录", nullptr, logoutKey, 0, ElaIconType::ArrowRightFromBracket);
    _modifyInfor_win=new modifyInfor_win();
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)
            {
                if (modifyKey == nodeKey)
                {
                    if(CurrentUser!=nullptr){
                        _modifyInfor_win->show();
                    }
                    else{
                        onMessage("请先登录!","Error");
                    }
                }
                else if(cancelKey==nodeKey)
                {
                    if(CurrentUser!=nullptr){
                    QWidget* _centralWidget = new QWidget(this);
                    QVBoxLayout* centralVLayout = new QVBoxLayout(_centralWidget);
                    centralVLayout->setContentsMargins(9, 15, 9, 20);
                    ElaText* title = new ElaText("注销账号", this);
                    title->setTextStyle(ElaTextType::Title);
                    ElaText* subTitle = new ElaText("确定要注销账号吗?一旦注销账号无法恢复！", this);
                    subTitle->setTextStyle(ElaTextType::Body);
                    centralVLayout->addWidget(title);
                    centralVLayout->addWidget(subTitle);
                    centralVLayout->addStretch();
                    ElaContentDialog *dialag = new ElaContentDialog(this,false);
                    dialag->setCentralWidget(_centralWidget);
                    dialag->setLeftButtonText("取消");
                    dialag->setRightButtonText("确认");
                    connect(dialag, &ElaContentDialog::rightButtonClicked, this, &MainWindow::onUserdelete);
                    dialag->show();}
                    else{
                        onMessage("请先登录!","Error");
                    }
                }
                else if(logoutKey==nodeKey)
                {
                    if(CurrentUser!=nullptr){
                    QWidget* _logoutWidget = new QWidget(this);
                    QVBoxLayout* logoutVLayout = new QVBoxLayout(_logoutWidget);
                    logoutVLayout->setContentsMargins(9, 15, 9, 20);
                    ElaText* logoutTitle = new ElaText("退出登录", this);
                    logoutTitle->setTextStyle(ElaTextType::Title);
                    ElaText* logoutSubTitle = new ElaText("确定要退出登录吗?", this);
                    logoutSubTitle->setTextStyle(ElaTextType::Body);
                    logoutVLayout->addWidget(logoutTitle);
                    logoutVLayout->addWidget(logoutSubTitle);
                    logoutVLayout->addStretch();
                    ElaContentDialog *logoutdialag = new ElaContentDialog(this,false);
                    logoutdialag->setCentralWidget(_logoutWidget);
                    logoutdialag->setLeftButtonText("取消");
                    logoutdialag->setRightButtonText("确认");
                    connect(logoutdialag, &ElaContentDialog::rightButtonClicked, this, &MainWindow::exitLogin);
                    logoutdialag->show();}
                    else{
                        onMessage("请先登录!","Error");
                    }
                }
    });

    qDebug() << ElaEventBus::getInstance()->getRegisteredEventsName();
    connect(login, &loginwin::on_login_complete, this, &MainWindow::onUserLoggedIn);
    connect(_modifyInfor_win, &modifyInfor_win::changexinxi, this, &MainWindow::onModifyInfo);
    // 拦截默认关闭事件
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, &MainWindow::onCloseButtonClicked);
    autologin();
}

MainWindow::~MainWindow() {}
void MainWindow::onUserLoggedIn(User user)
{
    CurrentUser=new User(user);
    connect(CurrentUser->channel,&MessageChannel::message,this,&MainWindow::onMessage);
    COSConfig cosConfig=CurrentUser->getS3Config();
    _modifyInfor_win->currentUser=CurrentUser;
    um->updateUserInfo(CurrentUser);
    setUserInfoCardTitle(user.getUsername());
    setUserInfoCardSubTitle(user.getEmail());
    _syncCore=new SyncCore(this);
    _syncTaskDatabaseManager=new SyncTaskDatabaseManager(CurrentUser);
    QString url=user.getAvatarPath();
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
    qDebug() << "Connecting taskTotalSize signal";
    connect(_syncCore,&SyncCore::taskTotalSize,this,&MainWindow::onTaskTotalSize);
    qDebug() << "Connecting taskUploadSize signal";
    connect(_syncCore,&SyncCore::taskUploadSize,this,&MainWindow::onTaskUploadSize);
    qDebug() << "Connecting addFileUploadTask signal";
    connect(_syncCore,&SyncCore::addFileUploadTask,this,&MainWindow::onFileUploadTaskCreated);
    qDebug() << "Connecting updateFileUploadTask signal";
    connect(_syncCore,&SyncCore::updateFileUploadTask,this,&MainWindow::onFileUploadTaskUpdated);
    qDebug() << "Connecting addFileDownloadTask signal";
    connect(_syncCore,&SyncCore::addFileDownloadTask,this,&MainWindow::onFileDownloadTaskCreated);
    qDebug() << "Connecting updateFileDownloadTask signal";
    connect(_syncCore,&SyncCore::updateFileDownloadTask,this,&MainWindow::onFileDownloadTaskUpdated);
    connect(_syncCore,&SyncCore::finishFileDownloadTask,this,&MainWindow::onFileDownloadTaskFinished);
    connect(_syncCore,&SyncCore::finishFileUploadTask,this,&MainWindow::onFileUploadTaskFinished);
    connect(_filemanagePage,&FileManagePage::deleteTask,[=](int taskId){
       this->_syncTaskDatabaseManager->deleteTask(taskId);
    });
    for (auto const &x:_syncTaskDatabaseManager->getTasks()){
        SyncTask* task=new SyncTask(x);
        TaskToken tt=CurrentUser->getTaskTokenByRemote(x.getRemotePath());
        QString bucketName=cosConfig.bucketName;
        QString appId=cosConfig.appId;
        QString region=cosConfig.region;
        QString secretId=tt.tmpSecretId;
        QString secretKey=tt.tmpSecretKey;
        QString token=tt.sessionToken;
        QDateTime expiredTime=tt.expiredTime;
        COSClient *cosclient=new COSClient(bucketName,appId,region,secretId,secretKey,token,expiredTime);
        task->cosclient=cosclient;
        if (x.getLastSyncTime()==QDateTime::fromString("2000-01-01 00:00:00","yyyy-MM-dd hh:mm:ss"))
        {
            QString timeDelta="从未同步";
            this->_filemanagePage->addDirCard(x.getLocalPath(),11,timeDelta,x.getId());
        }else{
            QString timeDelta=QString::number(x.getLastSyncTime().daysTo(QDateTime::currentDateTime()))+"天前";
            this->_filemanagePage->addDirCard(x.getLocalPath(),11,timeDelta,x.getId());
        }
        _syncCore->addTask(task);
    }

}

void MainWindow::exitLogin()
{
    if(CurrentUser->logout()){
        um->setAutoLoginStaus(false);
        setUserInfoCardTitle("未登录");
        setUserInfoCardSubTitle("");
        setUserInfoCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
        onMessage("退出账号成功","Success");
        CurrentUser=nullptr;
    }
    else{
        onMessage("退出账号失败","Error");
    }
}

void MainWindow::onNeedPassword(const QString &account)
{
    QString password = um->getUserPassWord(account);
    emit dbPassword(password);
}
/*
void MainWindow::insertUserToDatabase(User user)
{
    qDebug()<<user.getEmail()<<" "<<user.gethashedPassword();
    db->insertUser(user.getEmail(),user.gethashedPassword());
    qDebug()<<db->getUserPassword(user.getEmail());
}
*/

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

        if(_syncTaskDatabaseManager!=nullptr)
        {
            int res=_syncTaskDatabaseManager->addTask(task);
            bool isSuccess=false;
            if(_syncCore!=nullptr)
            {
                SyncTask mytask(task);
                SyncTask* task=new SyncTask(mytask);
                task->setId(res);
                TaskToken tt=CurrentUser->getTaskTokenByRemote(task->getRemotePath());
                QString bucketName="qsync";
                QString appId="1320107701";
                QString region="ap-nanjing";
                QString secretId=tt.tmpSecretId;
                QString secretKey=tt.tmpSecretKey;
                QString token=tt.sessionToken;
                QDateTime expiredTime=tt.expiredTime;
                COSClient *cosclient=new COSClient(bucketName,appId,region,secretId,secretKey,token,expiredTime);
                task->cosclient=cosclient;
                isSuccess=_syncCore->addTask(task);
            }
            if(isSuccess)
            {
                if (task.getLastSyncTime()==QDateTime::fromString("2000-01-01 00:00:00","yyyy-MM-dd hh:mm:ss"))
                {
                    QString timeDelta="从未同步";
                    this->_filemanagePage->addDirCard(task.getLocalPath(),111,timeDelta,task.getId());
                }else{
                    QString timeDelta=QString::number(task.getLastSyncTime().daysTo(QDateTime::currentDateTime()))+"天前";
                    this->_filemanagePage->addDirCard(task.getLocalPath(),1111,timeDelta,task.getId());
                }
                onMessage("新文件夹链接成功。","Success");
            }
            else
            {
                _syncTaskDatabaseManager->deleteTask(res);
            }
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
    um->updateUserInfo(CurrentUser);
    qDebug() << user.getEmail() << " " << user.gethashedPassword() <<" " << um->getUserPassWord(user.getEmail());
    setUserInfoCardTitle(_modifyInfor_win->newIdEdit_->text());
    setUserInfoCardSubTitle(user.getEmail());

    QString url=user.getAvatarPath();
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

void MainWindow::onUserdelete()
{
    if(CurrentUser->deleteAccount()){
        um->setAutoLoginStaus(false);
        setUserInfoCardTitle("未登录");
        setUserInfoCardSubTitle("");
        setUserInfoCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
        onMessage("注销账号成功","Success");
        CurrentUser=nullptr;
    }
    else{
        onMessage("注销账号失败","Error");
    }
}

void MainWindow::onFileUploadTaskCreated(const QString &localPath, int fileTaskId) {

    _syncingPage->addFile(localPath,0,0,0,fileTaskId);
    qDebug()<<"File upload task created"<<localPath<<" "<<fileTaskId;
}

void MainWindow::onFileUploadTaskUpdated(int fileTaskId, qint64 nowSize, qint64 totalSize) {
    qDebug()<<"File upload task updated"<<fileTaskId<<" "<<nowSize<<"/"<<totalSize;
    if(totalSize==nowSize){
        _syncingPage->removeFile(
            fileTaskId
            );
        return ;
    }
    _syncingPage->modifyFile(
        totalSize,
        nowSize,
        fileTaskId
        );
}

void MainWindow::onFileUploadTaskPaused(int fileTaskId) {
    qDebug()<<"File upload task paused"<<fileTaskId;
}

void MainWindow::onFileDownloadTaskCreated(const QString &localPath, int fileTaskId) {
    _syncingPage->addFile(localPath,0,0,0,fileTaskId);
    qDebug()<<"File download task created"<<localPath<<" "<<fileTaskId;
}

void MainWindow::onFileDownloadTaskUpdated(int fileTaskId, qint64 nowSize, qint64 totalSize) {
    qDebug()<<"File download task updated"<<fileTaskId<<" "<<nowSize<<"/"<<totalSize;
    if(totalSize==nowSize){
        _syncingPage->removeFile(
            fileTaskId
            );
        return ;
    }
    _syncingPage->modifyFile(
        totalSize,
        nowSize,
        fileTaskId
        );
}

void MainWindow::onFileDownloadTaskPaused(int fileTaskId) {
    qDebug()<<"File download task paused"<<fileTaskId;
}

void MainWindow::onTaskTotalSize(qint64 size, int taskid) {
    this->_syncTaskDatabaseManager->updateTaskTime(taskid,QDateTime::currentDateTime());
    QString timeFinished=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    this->_filemanagePage->modifyDirCard(size,timeFinished,taskid);
}

void MainWindow::onTaskUploadSize(qint64 size, int taskid) {
    this->_filemanagePage->modifyDirCard(size,"正在同步",taskid);
}

void MainWindow::onFileUploadTaskFinished(int fileTaskId)
{
    _syncingPage->removeFile(
        fileTaskId
        );
}

void MainWindow::onFileDownloadTaskFinished(int fileTaskId)
{
    _syncingPage->removeFile(
        fileTaskId
        );
}

void MainWindow::onUserPausedFileTask(int fileTaskId)
{

}

void MainWindow::autologin()
{
    if(um->getAutoLoginStaus()){
        QString account=um->getUserAccount();
        QString password=um->getUserPassWord(account);
        CurrentUser=new User(account,password);
        if(CurrentUser->login()){
            onUserLoggedIn(*CurrentUser);
            onMessage("自动登录成功！","Success");
        }else{
            //onMessage("自动登录失败！","Error");
            CurrentUser=nullptr;
        }
    }
}
