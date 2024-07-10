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
#include "loginwin.h"
#include "syncing_view.h"
#include"filemange_view.h"
#include "historysync_view.h"
#include"historyview.h"
#include "filefunc.h"
#include "qthread.h"
#include "modifyinfor_win.h"

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
{

    db = new DatabaseManager(this); // 创建数据库管理器实例
    db->initializeDatabase(); // 初始化数据库
    // ElaApplication::getInstance()->setThemeMode(ElaApplicationType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Minimal);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);
    setUserInfoCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    setUserInfoCardTitle("未登录");
    setUserInfoCardSubTitle("");
    setWindowTitle("珞珈云");
    //setIsStayTop(true);
    // setUserInfoCardVisible(false);
    _homePage = new HomePage(this);
    _syncingPage = new SyncingPage(this);
    _filemanagePage=new FileManagePage(this);
    _historysyncPage = new HistorysyncPage(this);
    _userinfopage = new UserInfoPage(this);
    _historyviewPage = new HistoryViewPage(this);

    connect(this, &ElaWindow::userInfoCardClicked, [=]() {
        if(CurrentUser==nullptr)
            login->show();
    });
    connect(this,&MainWindow::dbPassword,login,&loginwin::on_db_response);
    connect(login,&loginwin::needPassword,this,&MainWindow::onNeedPassword);
    connect(signin, &signinwin::on_signin_complete, this, &MainWindow::insertUserToDatabase);
    qDebug()<<connect(login->channel,&MessageChannel::message,this,&MainWindow::onMessage);
    connect(_filemanagePage->linknewfolderwindow,&linkNewFolder_window::onNewTask,this,&MainWindow::onUserAddNewTask);
    // GraphicsView
    ElaGraphicsScene *scene = new ElaGraphicsScene(this);
    scene->setSceneRect(0, 0, 1500, 1500);
    // scene->setSceneRect(0, 0, 1000, 1000);
    ElaGraphicsItem *item1 = new ElaGraphicsItem();
    item1->setWidth(100);
    item1->setHeight(100);
    ElaGraphicsItem *item2 = new ElaGraphicsItem();
    item2->setWidth(100);
    item2->setHeight(100);
    // ElaGraphicsItem* item3 = new ElaGraphicsItem();
    // item3->setWidth(100);
    // item3->setHeight(100);
    // item3->setPos(10, 10);
    scene->addItem(item1);
    scene->addItem(item2);
    // scene->addItem(item3);
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
    addExpanderNode("个人功能",testKey_4,ElaIconType::User);
    addPageNode("修改信息",_userinfopage,testKey_4,ElaIconType::Text);
    addPageNode("注销账号",new QWidget(this),testKey_4,ElaIconType::UserSlash);
    addPageNode("退出登录",new QWidget(this),testKey_4,ElaIconType::ArrowRightFromBracket);

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
                    //
                }
    });



    // 下拉菜单
    addPageNode("HOME", _homePage, ElaIconType::House);
    addExpanderNode("ElaDxgi", _elaDxgiKey, ElaIconType::TvMusic);
    // addPageNode("ElaScreen", _elaScreenPage, _elaDxgiKey, 3, ElaIconType::ObjectGroup);
    //  navigation(elaScreenWidget->property("ElaPageKey").toString());
    // addPageNode("ElaBaseComponents", _baseComponentsPage, ElaIconType::CabinetFiling);
    // addPageNode("ElaGraphics", view, 9, ElaIconType::KeySkeleton);
    // addPageNode("ElaIcon", _iconPage, 99, ElaIconType::FontAwesome);
    // addPageNode("ElaTabWidget", _tabWidgetPage, ElaIconType::Table);
    /*
    addExpanderNode("TEST4", testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST5", testKey_1, testKey_2, ElaIconType::Acorn);
    addPageNode("Third Level", new QWidget(this), testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST6", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST7", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST8", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST9", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST10", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST11", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST12", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST13", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST14", testKey_1, testKey_2, ElaIconType::Acorn);
    addExpanderNode("TEST15", testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST16", testKey_1, ElaIconType::Acorn);
    addExpanderNode("TEST17", testKey_1, ElaIconType::Acorn);
*/
    /*
    addFooterNode("About", nullptr, _aboutKey, 0, ElaIconType::User);
    ElaWidget *widget = new ElaWidget();
    widget->setWindowModality(Qt::ApplicationModal);
    widget->setCentralWidget(new QWidget());
    widget->hide();
    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey)
            {
        if (_aboutKey == nodeKey)
        {
            widget->show();
        } });
    addFooterNode("Setting", new QWidget(this), _settingKey, 0, ElaIconType::GearComplex);
    */
    /*
    connect(this, &MainWindow::userInfoCardClicked, this, [=]()
            { this->navigation(_homePage->property("ElaPageKey").toString()); });
            */
    /*
    connect(_homePage, &HomePage::elaScreenNavigation, this, [=]() {
        this->navigation(_elaScreenPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &HomePage::elaBaseComponentNavigation, this, [=]() {
        this->navigation(_baseComponentsPage->property("ElaPageKey").toString());
    });
    connect(_homePage, &HomePage::elaSceneNavigation, this, [=]() {
        this->navigation(view->property("ElaPageKey").toString());
    });

    connect(_homePage, &HomePage::elaIconNavigation, this, [=]() {
        this->navigation(_iconPage->property("ElaPageKey").toString());
    });
*/
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
    _userinfopage->currentUser=CurrentUser;
    _modifyInfor_win->currentUser=CurrentUser;
    db->insertUser(user.getEmail(),user.gethashedPassword());
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
}

void MainWindow::onNeedPassword(const QString &account)
{
    QString password = db->getUserPassword(account).second;
    emit dbPassword(password);
}

void MainWindow::insertUserToDatabase(User user)
{
    db->insertUser(user.getEmail(),user.gethashedPassword());
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
    db->updateUserInfo(user.getEmail(),_modifyInfor_win->newPasswordEdit_->text());
    qDebug() << user.getEmail() << " " << db->getUserPassword(user.getEmail());
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
