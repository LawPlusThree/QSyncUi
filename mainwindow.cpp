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
#include "ElaWidget.h"
#include "homeView.h"

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
{
    // ElaApplication::getInstance()->setThemeMode(ElaApplicationType::Dark);
    // setIsNavigationBarEnable(false);
    // setNavigationBarDisplayMode(ElaNavigationType::Minimal);
    // setWindowButtonFlag(ElaAppBarType::MinimizeButtonHint, false);
    setUserInfoCardPixmap(QPixmap(":/include/Image/Cirno.jpg"));
    setUserInfoCardTitle("未登录");
    setUserInfoCardSubTitle("");
    setWindowTitle("ElaWidgetTool");
    // setIsStayTop(true);
    //  setUserInfoCardVisible(false);
    _homePage = new HomePage(this);

    connect(this, &ElaWindow::userInfoCardClicked, this, [=]()
            { login->show(); });

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

    addExpanderNode("同步功能", testKey_2, ElaIconType::House);
    addPageNode("正在同步", new QWidget(this), testKey_2, ElaIconType::Cloud);
    addPageNode("历史同步", new QWidget(this), testKey_2, ElaIconType::CheckToSlot);
    addPageNode("同步文件夹管理", new QWidget(this), testKey_2, ElaIconType::FolderClosed);
    addExpanderNode("版本控制", testKey_3, ElaIconType::EnvelopeOpenText);
    addPageNode("查看历史", new QWidget(this), testKey_3, ElaIconType::CalendarClock);
    addExpanderNode("个人功能", testKey_4, ElaIconType::User);
    addPageNode("修改信息", new QWidget(this), testKey_4, ElaIconType::Text);
    addPageNode("注销账号", new QWidget(this), testKey_4, ElaIconType::UserSlash);
    addPageNode("退出登录", new QWidget(this), testKey_4, ElaIconType::ArrowRightFromBracket);

    // 下拉菜单
    addPageNode("HOME", _homePage, ElaIconType::House);
    addExpanderNode("ElaDxgi", _elaDxgiKey, ElaIconType::TvMusic);
    // addPageNode("ElaScreen", _elaScreenPage, _elaDxgiKey, 3, ElaIconType::ObjectGroup);
    //  navigation(elaScreenWidget->property("ElaPageKey").toString());
    // addPageNode("ElaBaseComponents", _baseComponentsPage, ElaIconType::CabinetFiling);
    // addPageNode("ElaGraphics", view, 9, ElaIconType::KeySkeleton);
    // addPageNode("ElaIcon", _iconPage, 99, ElaIconType::FontAwesome);
    // addPageNode("ElaTabWidget", _tabWidgetPage, ElaIconType::Table);
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
    connect(this, &MainWindow::userInfoCardClicked, this, [=]()
            { this->navigation(_homePage->property("ElaPageKey").toString()); });
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
    QObject::connect(loginWindow, &LoginWindow::on_login_complete, mainWindow, &MainWindow::onUserLoggedIn);
    // 拦截默认关闭事件
    this->setIsDefaultClosed(false);
    connect(this, &MainWindow::closeButtonClicked, this, &MainWindow::onCloseButtonClicked);
}

MainWindow::~MainWindow() {}
void MainWindow::onUserLoggedIn(User user)
{
    setUserInfoCardTitle(user.getUsername());
}

void MainWindow::onCloseButtonClicked()
{
    ElaContentDialog *dialag = new ElaContentDialog(this);
    connect(dialag, &ElaContentDialog::rightButtonClicked, this, &MainWindow::closeWindow);
    connect(dialag, &ElaContentDialog::middleButtonClicked, this, &MainWindow::showMinimized);
    dialag->show();
}
