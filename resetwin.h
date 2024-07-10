#ifndef RESETWIN_H
#define RESETWIN_H

#include"ElaWidget.h"
#include"ElaLineEdit.h"

class ElaInteractiveCard;
class QHBoxLayout;
class resetwin:public ElaWidget
{
    Q_OBJECT
public:
    explicit resetwin(QWidget *parent = nullptr);
    ~resetwin();
    ElaInteractiveCard*avatar;
    QHBoxLayout*avatarArea;

signals:
    void goback();
public:
    ElaLineEdit*passwordLine;
    ElaLineEdit*passagainLine;
private slots:
    void on_resetBtn_clicked();
    void closeEvent(QCloseEvent*Event);

};

#endif // RESETWIN_H
