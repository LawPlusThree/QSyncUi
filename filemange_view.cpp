#include "filemange_view.h"

#include<QHBoxLayout>
#include"ElaWidget.h"
#include"ElaText.h"
#include"ElaPushButton.h"
#include"ElaToggleButton.h"
#include"ElaScrollArea.h"
#include"ElaCheckBox.h"
#include"ElaComboBox.h"

#include"DirCard.h"
#include"dircardproxy.h"

FileManagePage::FileManagePage(QWidget* parent,UserManager *um):ElaScrollPage(parent)
{
    /*ElaWidget*topCard=new ElaWidget();
    topCard->setMaximumHeight(50);
    QVBoxLayout*topCardArea=new QVBoxLayout(topCard);*/

    setexcludeditemsview=new setExcludedItems_view(nullptr,um);

    QWidget* centralWidget = new QWidget(this); // 创建中心部件
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget); // 为中心部件设置垂直布局
    centerVLayout->setContentsMargins(0, 0, 5, 0); // 设置布局边距,5为到右边的距离
    centralWidget->setWindowTitle("同步文件夹管理"); // 设置窗口标题

    /*// 创建一个 ElaProgressBar 对象，用作进度条
    _progressBar = new ElaProgressBar(this);
    // 设置QProgressBar的大小策略为水平方向可扩展，垂直方向固定
    _progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 设置最小尺寸，可以适当调整宽度以减少填充效果
    _progressBar->setMinimumSize(100, 20); // 调整最小宽度为100，最小高度20
    // 设置进度条的最小值和最大值
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(0); // 表示不确定的进度
    QWidget* progressBarArea = new QWidget();
    progressBarArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    progressBarArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // 创建一个 ElaScrollPageArea 对象，用作进度条的容器
    //ElaScrollPageArea* progressBarArea = new ElaScrollPageArea(this);
    // 为进度条容器设置水平布局
    QHBoxLayout* progressBarLayout = new QHBoxLayout(progressBarArea);
    progressBarLayout->setContentsMargins(50, 0, 55, 0); // 设置布局的边距
    // 创建一个 ElaText 对象，用于显示进度条的文本标签
    ElaText* progressBarText = new ElaText("总进度：", this);
    // 设置进度条文本标签的字体大小为 15
    progressBarText->setTextSize(15);
    // 将进度条文本标签和进度条控件添加到布局中
    progressBarLayout->addWidget(progressBarText);
    progressBarLayout->addWidget(_progressBar);*/
    // 创建一个 ElaToggleButton 对象，设置其标签为 "Head" 并指定其父对象
    _pushButton1 = new ElaPushButton("解除绑定", this);
    _pushButton1->setFixedSize(100, 40); // 设置按钮的固定大小
    //_toggleButton = new ElaToggleButton("暂时停止", this);
    //_toggleButton->setFixedSize(100, 40); // 设置按钮的固定大小
    _pushButton2 = new ElaPushButton("设置排除项目", this);
    _pushButton2->setFixedSize(100, 40); // 设置按钮的固定大小
    _pushButton3 = new ElaPushButton("链接新文件夹", this);
    _pushButton3->setFixedSize(100, 40); // 设置按钮的固定大小

    _comboBox = new ElaComboBox(this);
    QStringList comboList{"1", "2", "3", "4", "5", "6", "7"};
    _comboBox->addItems(comboList);
    _comboBox->setFixedSize(60,30);
    _comboBox->setCurrentIndex(2);
    QWidget* comboBoxArea = new QWidget(this);
    comboBoxArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    comboBoxArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    QHBoxLayout* comboBoxLayout = new QHBoxLayout(comboBoxArea);
    ElaText* comboBoxText = new ElaText("线程数:", this);
    comboBoxText->setTextSize(15);
    comboBoxLayout->addWidget(comboBoxText);
    comboBoxLayout->addWidget(_comboBox);
    connect(_comboBox, &ElaComboBox::currentIndexChanged, [this](int index) {
        qDebug() << "当前选中的索引为：" << index;
        qDebug() << "当前选中的文本为：" << _comboBox->currentText();
        emit setThreadNum(_comboBox->currentText().toInt());
    });
    connect(_comboBox, &ElaComboBox::currentIndexChanged, [=](int index) {
        um->setThread(index+1);
    });

    QWidget* pushButtonArea = new QWidget();
    pushButtonArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    pushButtonArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // 创建一个 ElaScrollPageArea 对象，用作切换按钮的容器，并指定其父对象
    //ElaScrollPageArea* pushButtonArea = new ElaScrollPageArea(this);
    // 为切换按钮容器设置水平布局
    QHBoxLayout* pushButtonLayout = new QHBoxLayout(pushButtonArea);
    pushButtonLayout->setContentsMargins(45, 0, 45, 0);
    // 将切换按钮控件添加到布局中
    //pushButtonLayout->addWidget(_toggleButton);
    pushButtonLayout->addWidget(_pushButton2);
    pushButtonLayout->addWidget(_pushButton3);
    // 在布局中添加一个弹性空间，使得所有控件靠左对齐
    pushButtonLayout->addStretch();
    pushButtonLayout->addWidget(_pushButton1);
    pushButtonLayout->addStretch();
    pushButtonLayout->addWidget(comboBoxArea);
    connect(_pushButton1,&ElaPushButton::clicked,[=](){
        removeChecked();
    });
    connect(_pushButton2,&ElaPushButton::clicked,[=](){
        setexcludeditemsview->show();
    });
    connect(_pushButton3,&ElaPushButton::clicked,[=](){
        linknewfolderwindow->show();
    });
    _pushButton1->hide();

    QWidget* catalogueArea = new QWidget();
    catalogueArea->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    catalogueArea->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    //ElaScrollPageArea* catalogueArea = new ElaScrollPageArea(this);
    QHBoxLayout* catalogueLayout = new QHBoxLayout(catalogueArea);
    catalogueLayout->setContentsMargins(18, 0, 20, 0);
    ElaText* catalogueText0 = new ElaText("", this);
    catalogueText0->setTextSize(16);
    catalogueText0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText0->setFixedSize(25,20);
    catalogueText0->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea0 = new QVBoxLayout();
    catalogueArea0->addWidget(catalogueText0, 0, Qt::AlignCenter);

    ElaText* catalogueText1 = new ElaText("文件夹名", this);
    catalogueText1->setTextSize(16);
    catalogueText1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    //QWidget* filenameWidget = new QWidget();
    filenameWidget->setWindowFlags(Qt::FramelessWindowHint); // 去除窗口边框
    filenameWidget->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    filenameWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    QVBoxLayout* filenameArea = new QVBoxLayout(filenameWidget);
    filenameArea->addWidget(catalogueText1, 0, Qt::AlignLeft);

    ElaText* catalogueText2 = new ElaText("数据大小", this);
    catalogueText2->setTextSize(16);
    catalogueText2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText2->setFixedSize(100,20);
    catalogueText2->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea2 = new QVBoxLayout();
    catalogueArea2->addWidget(catalogueText2, 0, Qt::AlignCenter);
    ElaText* catalogueText4 = new ElaText("同步状态", this);
    catalogueText4->setTextSize(16);
    catalogueText4->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText4->setFixedSize(140,20);
    catalogueText4->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QVBoxLayout* catalogueArea4 = new QVBoxLayout();
    catalogueArea4->addWidget(catalogueText4, 0, Qt::AlignCenter);
    ElaText* catalogueText5 = new ElaText("操作", this);
    catalogueText5->setTextSize(16);
    catalogueText5->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    catalogueText5->setFixedSize(60,20);
    catalogueText5->setAlignment(Qt::AlignCenter); // 设置文本居中对齐
    QHBoxLayout* catalogueArea5 = new QHBoxLayout();
    catalogueArea5->addWidget(catalogueText5, 0, Qt::AlignCenter);
    catalogueLayout->addLayout(catalogueArea0);
    catalogueLayout->addWidget(filenameWidget);
    catalogueLayout->addLayout(catalogueArea2);
    catalogueLayout->addLayout(catalogueArea4);
    catalogueLayout->addLayout(catalogueArea5);
    catalogueLayout->setStretchFactor(catalogueArea0, 25);
    catalogueLayout->setStretchFactor(filenameWidget, 500);
    catalogueLayout->setStretchFactor(catalogueArea2, 100);
    catalogueLayout->setStretchFactor(catalogueArea4, 140);
    catalogueLayout->setStretchFactor(catalogueArea5, 60);

    // 创建QScrollArea用于包裹files
    ElaScrollArea* scrollArea = new ElaScrollArea();
    scrollArea->viewport()->setStyleSheet("background:transparent;");//设置背景透明

    _dircardProxy=new DirCardProxy(this);

    scrollArea->setWidget(_dircardProxy);
    scrollArea->setWidgetResizable(true); // 允许scrollArea根据内容自动调整大小

    connect(_dircardProxy, &DirCardProxy::checkBoxToggled, this, &FileManagePage::buttonShowHide);
    connect(_dircardProxy, &DirCardProxy::removeCard, this, &FileManagePage::buttonShowHide);
    //centerVLayout->addWidget(progressBarArea); // 将上方固定区域添加到布局中
    centerVLayout->addWidget(pushButtonArea); // 将切换按钮容器添加到布局中
    centerVLayout->addWidget(catalogueArea); // 将目录文本添加到布局中
    centerVLayout->addWidget(scrollArea); // 将scrollArea添加到布局中

    this->addCentralWidget(centralWidget); // 将中心部件添加到窗口

}

FileManagePage::~FileManagePage()
{

}

void FileManagePage::addDirCard(QString filename,QString cloudname,quint64 datasize,QString bindtime,int syncStatus,int id)
{
    DirCard*newDir=new DirCard(filename,cloudname,datasize,bindtime,syncStatus,id);
    connect(newDir,&DirCard::relieve,this,&FileManagePage::removeDirCard);
    _dircardProxy->addDirCard(newDir,id);

    QFontMetrics metrics(newDir->filename->font());
    QString elidedText = metrics.elidedText(newDir->fullText, Qt::ElideMiddle, filenameWidget->width()/2-35);
    newDir->filename->setText(elidedText);
    newDir->filename->setToolTip(newDir->fullText);

    QFontMetrics metrics2(newDir->filename->font());
    QString elidedText2 = metrics2.elidedText(newDir->cfullText, Qt::ElideMiddle, filenameWidget->width()/2-35);
    newDir->cloudname->setText(elidedText2);
    newDir->cloudname->setToolTip(newDir->cfullText);
}

void FileManagePage::removeDirCard(int id)
{
    _dircardProxy->removeDirCard(id);
    emit deleteTask(id);
}

void FileManagePage::removeChecked()
{
    QMapIterator<int, DirCard*> i(_dircardProxy->cardMap);
    while (i.hasNext())
    {
        i.next();
        int id = i.key();
        DirCard *card = i.value();
        if(card->ischecked())
            removeDirCard(id);
    }
}

void FileManagePage::modifyDirCard(quint64 datasize,QString bindtime,int id)
{
    _dircardProxy->modifyDirCard(datasize,bindtime,id);
}

void FileManagePage::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    auto thisMap=this->_dircardProxy->cardMap;
    for (auto &x:thisMap){
        QFontMetrics metrics(x->filename->font());
        QString elidedText = metrics.elidedText(x->fullText, Qt::ElideMiddle, filenameWidget->width()/2-35);
        x->filename->setText(elidedText);
        x->filename->setToolTip(x->fullText);

        QFontMetrics metrics2(x->filename->font());
        QString elidedText2 = metrics2.elidedText(x->cfullText, Qt::ElideMiddle, filenameWidget->width()/2-35);
        x->cloudname->setText(elidedText2);
        x->cloudname->setToolTip(x->cfullText);
    }
}

void FileManagePage::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    auto thisMap=this->_dircardProxy->cardMap;
    for (auto &x:thisMap){
        QFontMetrics metrics(x->filename->font());
        QString elidedText = metrics.elidedText(x->fullText, Qt::ElideMiddle, filenameWidget->width()/2-35);
        x->filename->setText(elidedText);
        x->filename->setToolTip(x->fullText);

        QFontMetrics metrics2(x->filename->font());
        QString elidedText2 = metrics2.elidedText(x->cfullText, Qt::ElideMiddle, filenameWidget->width()/2-35);
        x->cloudname->setText(elidedText2);
        x->cloudname->setToolTip(x->cfullText);
    }
}

void FileManagePage::updateComboBoxIndex(int index) {
    if (index-1 >= 0 && index-1 < _comboBox->count()) {
        _comboBox->setCurrentIndex(index-1);
    }
}

void FileManagePage::buttonShowHide()
{
    QMapIterator<int, DirCard*> i(_dircardProxy->cardMap);
    bool allNotChecked = true;
    while (i.hasNext())
    {
        i.next();
        DirCard *card = i.value();
        if(card->ischecked())
        {
            allNotChecked = false;
            break;
        }
    }
    if(!allNotChecked) {
        _pushButton1->show(); // 如果复选框被勾选，显示按钮
    } else {
        _pushButton1->hide(); // 如果复选框未被勾选，隐藏按钮
    }
}
