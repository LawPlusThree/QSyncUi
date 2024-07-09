#ifndef ELAWIDGET_H
#define ELAWIDGET_H

#include <QWidget>

#include "stdafx.h"
#include "ElaAppBar.h"

class ElaWidgetPrivate;
class ELA_EXPORT ElaWidget : public QWidget
{
    Q_OBJECT
    Q_Q_CREATE(ElaWidget)
public:
    explicit ElaWidget(QWidget* parent = nullptr,int wi=500,int he=500);
    ~ElaWidget();
    void setCentralWidget(QWidget* centralWidget);

    void setWindowTitle(QString title);
    void setIsStayTop(bool isStayTop);
    bool getIsStayTop() const;
    void setIsFixedSize(bool isFixedSize);
    bool getIsFixedSize() const;

    void setWindowButtonFlag(ElaAppBarType::ButtonType buttonFlag, bool isEnable = true);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // ELAWIDGET_H
