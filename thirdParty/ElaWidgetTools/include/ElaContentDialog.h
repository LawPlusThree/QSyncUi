#ifndef ELACONTENTDIALOG_H
#define ELACONTENTDIALOG_H
#include <QAbstractNativeEventFilter>
#include <QDialog>

#include "stdafx.h"

class ElaContentDialogPrivate;
class ElaPushButton;
class ELA_EXPORT ElaContentDialog : public QDialog, QAbstractNativeEventFilter
{
    Q_OBJECT
    Q_Q_CREATE(ElaContentDialog)
public:
    explicit ElaContentDialog(QWidget* parent = nullptr,bool MiddleButton=true);
    ~ElaContentDialog();
    ElaPushButton* getButton(); // 假设的方法，返回内部的复选框
    Q_SLOT virtual void onLeftButtonClicked();
    Q_SLOT virtual void onMiddleButtonClicked();
    Q_SLOT virtual void onRightButtonClicked();
    void setCentralWidget(QWidget* centralWidget);

    void setLeftButtonText(QString text);
    void setMiddleButtonText(QString text);
    void setRightButtonText(QString text);
Q_SIGNALS:
    Q_SIGNAL void leftButtonClicked();
    Q_SIGNAL void middleButtonClicked();
    Q_SIGNAL void rightButtonClicked();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
#if (QT_VERSION == QT_VERSION_CHECK(6, 5, 3) || QT_VERSION == QT_VERSION_CHECK(6, 6, 0))
    bool eventFilter(QObject* obj, QEvent* event) override;
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    virtual bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;
#else
    virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;
#endif
};

#endif // ELACONTENTDIALOG_H
