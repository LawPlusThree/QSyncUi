#include "mainwindow.h"

#include <QGraphicsView>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>

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
    _filemanagePage=new FileManagePage(this,this->um);
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
                    if(CurrentUser!=nullptr)
                    {
                    QWidget* _centralWidget = new QWidget(this);
                    QVBoxLayout* centralVLayout = new QVBoxLayout(_centralWidget);
                    centralVLayout->setContentsMargins(9, 15, 9, 20);
                    ElaText* title = new ElaText("注销账号", this);
                    title->setTextStyle(ElaTextType::Title);
                    ElaText* subTitle = new ElaText("确定要注销账号吗?一旦注销账号无法恢复！", this);
                    subTitle->setTextStyle(ElaTextType::Body);
                    cancelcheckBox = new ElaCheckBox("我已了解注销账号的后果，并确认继续。", this);
                    //cancelcheckBox->setStyleSheet("ElaCheckBox { font-size: 14px; }");
                    centralVLayout->addWidget(title);
                    centralVLayout->addWidget(subTitle);
                    centralVLayout->addWidget(cancelcheckBox);
                    centralVLayout->addStretch();
                    ElaContentDialog *dialag = new ElaContentDialog(this,false);
                    dialag->setCentralWidget(_centralWidget);
                    dialag->setLeftButtonText("取消");
                    dialag->setRightButtonText("确认");
                    dialag->getButton()->setEnabled(false);
                    dialag->getButton()->setStyleSheet("QPushButton { background-color: #B58C8C; color: white; }");
                    connect(cancelcheckBox, &ElaCheckBox::stateChanged, [=](int state) {
                        if (state == Qt::Checked)
                        {
                            dialag->getButton()->setStyleSheet("QPushButton { background-color: #FF6B5D; }"
                                                               "QPushButton:hover { background-color: red; }"
                                                               "QPushButton:pressed { background-color: red; }");
                            dialag->getButton()->setEnabled(true);
                        }
                        else
                        {
                            dialag->getButton()->setStyleSheet("QPushButton { background-color: #B58C8C; }");
                            dialag->getButton()->setEnabled(false);
                        }
                    });
                    connect(dialag, &ElaContentDialog::rightButtonClicked, [=]() {
                        onUserdelete();
                    });
                    dialag->show();
                    }
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
                else if(feedbackKey==nodeKey)
                {
                    QUrl surveyUrl("https://www.wjx.cn/vm/mnzxQPj.aspx/");
                    QDesktopServices::openUrl(surveyUrl);
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

MainWindow::MainWindow(QString action, QVector<QString> argv, QWidget *parent):
    MainWindow(parent)
{
    if (CurrentUser==nullptr){
        onMessage("请先登录","Error");
    _action=action;
    _argv=argv;
    login->show();
    }
    else{
        ArgvProcess(action,argv);
    }
}

void MainWindow::ArgvProcess(QString action, QVector<QString> argv)
{
    onMessage(action,"Info");
    onMessage(argv.join(" "),"Info");
    if (action=="ver"){
        this->navigation(this->_historyviewPage->property("ElaPageKey").toString());
    }
}

MainWindow::~MainWindow()
{
    delete um;
    delete _syncingPage;
    delete _filemanagePage;
    delete _historysyncPage;
    delete _historyviewPage;
    delete _modifyInfor_win;
    delete CurrentUser;
    delete _syncCore;
    delete _syncTaskDatabaseManager;
    delete _taskManager;
}

void MainWindow::onUserLoggedIn(User user)
{
    CurrentUser=new User(user);
    connect(CurrentUser->channel,&MessageChannel::message,this,&MainWindow::onMessage);
    _modifyInfor_win->currentUser=CurrentUser;
    QList<QString>s3dirs = CurrentUser->getS3Dirs();
    _filemanagePage->linknewfolderwindow->setItems(s3dirs);
    um->updateUserInfo(CurrentUser);
    setUserInfoCardTitle(user.getUsername());
    setUserInfoCardSubTitle(user.getEmail());

    _syncTaskDatabaseManager=new SyncTaskDatabaseManager(CurrentUser);
    _taskManager=new TaskManager(CurrentUser->getEmail());

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
    COSConfig cosConfig=CurrentUser->getS3Config();
    TaskToken tt=CurrentUser->getUnifiedTaskToken();
    cosConfig.taskToken=tt;
    _syncCore=new SyncCore(cosConfig,this);
    _syncCore->requestManager->setMaxConcurrentRequests(um->getThread());
    this->_filemanagePage->updateComboBoxIndex(um->getThread());
    qDebug() << "Connecting taskTotalSize signal";
    connect(_syncCore,&SyncCore::taskTotalSize,this,&MainWindow::onTaskTotalSize);
    qDebug() << "Connecting taskUploadSize signal";
    connect(_syncCore,&SyncCore::taskUploadSize,this,&MainWindow::onTaskUploadSize);
    qDebug() << "Connecting addFileUploadTask signal";
    connect(_syncCore,&SyncCore::addFileUploadTask,this,&MainWindow::onFileUploadTaskCreated,Qt::QueuedConnection);
    qDebug() << "Connecting updateFileUploadTask signal";
    connect(_syncCore,&SyncCore::updateFileUploadTask,this,&MainWindow::onFileUploadTaskUpdated,Qt::QueuedConnection);
    qDebug() << "Connecting addFileDownloadTask signal";
    connect(_syncCore,&SyncCore::addFileDownloadTask,this,&MainWindow::onFileDownloadTaskCreated,Qt::QueuedConnection);
    qDebug() << "Connecting updateFileDownloadTask signal";
    connect(_syncCore,&SyncCore::updateFileDownloadTask,this,&MainWindow::onFileDownloadTaskUpdated,Qt::QueuedConnection);
    connect(_syncCore,&SyncCore::finishFileDownloadTask,this,&MainWindow::onFileDownloadTaskFinished,Qt::QueuedConnection);
    connect(_syncCore,&SyncCore::finishFileUploadTask,this,&MainWindow::onFileUploadTaskFinished,Qt::QueuedConnection);
    connect(_filemanagePage,&FileManagePage::deleteTask,[=](int taskId){
        this->_syncTaskDatabaseManager->deleteTask(taskId);
    });
    connect(_filemanagePage,&FileManagePage::setThreadNum,[=](int num){
        this->_syncCore->requestManager->setMaxConcurrentRequests(num);
    });

    ReadUpTask();
    ReadDownTask();
    ReadFinishTask();
    
    for (auto const &x:_syncTaskDatabaseManager->getTasks()){
        SyncTask* task=new SyncTask(x);

        if (x.getLastSyncTime()==QDateTime::fromString("2000-01-01 00:00:00","yyyy-MM-dd hh:mm:ss"))
        {
            QString timeDelta="从未同步";
            this->_filemanagePage->addDirCard(x.getLocalPath(),x.getRemotePath(),11,timeDelta,task->getSyncStatus(),x.getId());
        }else{
            QString timeDelta=QString::number(x.getLastSyncTime().daysTo(QDateTime::currentDateTime()))+"天前";
            this->_filemanagePage->addDirCard(x.getLocalPath(),x.getRemotePath(),11,timeDelta,task->getSyncStatus(),x.getId());
        }
        _syncCore->addTask(task);
    }
    QVector<QString> s3Dirs = CurrentUser->getS3Dirs();
    qDebug() << "S3 Dirs:";
    for (const QString& s3Dir : s3Dirs) {
        qDebug() << s3Dir;
    }

    if(_action!=""){
        ArgvProcess(_action,_argv);
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

void MainWindow::onUserAddNewTask(const SyncTask &_task)
{
    if(CurrentUser==nullptr)
    {
        onMessage("请先登录","Error");
        return;
    }
    if(CurrentUser->addTask(_task.getLocalPath(),_task.getRemotePath(),_task.getSyncStatus(),1,1)){
        if(_syncTaskDatabaseManager!=nullptr)
        {
            int res=_syncTaskDatabaseManager->addTask(_task);
            bool isSuccess=false;
            TaskToken tt=CurrentUser->getUnifiedTaskToken();
            COSConfig cosConfig=CurrentUser->getS3Config();
            cosConfig.taskToken=tt;
            SyncTask* task=nullptr;
            if(_syncCore!=nullptr)
            {
                SyncTask mytask(_task);
                task=new SyncTask(mytask);
                task->setId(res);
                QDateTime expiredTime=tt.expiredTime;
                COSClient *cosclient=new COSClient(cosConfig, this);
                task->cosclient=cosclient;
                isSuccess=_syncCore->addTask(task);
            }
            if(isSuccess)
            {
                if (_task.getLastSyncTime()==QDateTime::fromString("2000-01-01 00:00:00","yyyy-MM-dd hh:mm:ss"))
                {
                    QString timeDelta="从未同步";
                    this->_filemanagePage->addDirCard(task->getLocalPath(),task->getRemotePath(),111,timeDelta,task->getSyncStatus(),task->getId());
                }else{
                    QString timeDelta=QString::number(task->getLastSyncTime().daysTo(QDateTime::currentDateTime()))+"天前";
                    this->_filemanagePage->addDirCard(task->getLocalPath(),task->getRemotePath(),1111,timeDelta,task->getSyncStatus(),task->getId());
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

    QFile file(localPath);
    quint64 size=file.size();
    _syncingPage->addFile(localPath,size,0,0,1,fileTaskId);
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

void MainWindow::onFileDownloadTaskCreated(const QString &localPath, int fileTaskId,quint64 totalSize) {
    _syncingPage->addFile(localPath,totalSize,0,0,2,fileTaskId);
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
    //this->_filemanagePage->modifyDirCard(size,"同步完成",taskid);
}

void MainWindow::onFileUploadTaskFinished(int fileTaskId)
{
    _syncingPage->removeFile(
        fileTaskId
        );
    onMessage("上传完成","Info");
}

void MainWindow::onFileDownloadTaskFinished(int fileTaskId)
{
    _syncingPage->removeFile(
        fileTaskId
        );
    onMessage("下载完成","Info");
}

void MainWindow::onUserPausedFileTask(int fileTaskId)
{

}

void MainWindow::ReadUpTask()
{

}

void MainWindow::ReadDownTask()
{

}

void MainWindow::ReadFinishTask()
{
    for (auto const &task:_taskManager->readFinishTask()){
        this->_historysyncPage->addHistory(task.localPath,QString::number(task.dataSize),task.sycnTime.toString("yyyy-MM-dd"),task.status==1?true:false);
    }
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
