#ifndef RESET_H
#define RESET_H

#include <QWidget>

namespace Ui {
class reset;
}

class reset : public QWidget
{
    Q_OBJECT

public:
    explicit reset(QWidget *parent = nullptr);
    ~reset();

signals:
    void goback();

private slots:
    void closeEvent(QCloseEvent*Event);

    void on_resetBtn_clicked();

private:
    Ui::reset *ui;
};

#endif // RESET_H
