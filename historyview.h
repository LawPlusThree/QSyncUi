#ifndef HISTORYVIEW_H
#define HISTORYVIEW_H

#include "ElaScrollPage.h"
#include"ElaProgressBar.h"

class HistoryviewCardProxy;
class QVBoxLayout;
class HistoryViewPage : public ElaScrollPage
{
public:
    explicit HistoryViewPage(QWidget* parent);
    QVBoxLayout*filesLayout;
    void addHistoryViewCard(QString filename,QString datasize);
    void addSubCard(QString filename,QString versionID,quint64 datasize,QString bindtime);
    QWidget* filenameWidget = new QWidget();
    QWidget* cloudnameWidget = new QWidget();
    HistoryviewCardProxy*_historyviewcardPage;

private:
    ElaProgressBar* _progressBar{nullptr};
protected:
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
};

#endif // HISTORYVIEW_H
