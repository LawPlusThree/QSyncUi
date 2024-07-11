#ifndef SYNCING_VIEW_H
#define SYNCING_VIEW_H

#include "ElaProgressBar.h"
#include "ElaScrollPage.h"
#include "ElaScrollPage.h"
#include "filecard.h"

class ElaToggleButton;
class ElaPushButton;
class QVBoxLayout;
class SyncingPage : public ElaScrollPage
{
public:
    SyncingPage(QWidget* parent = nullptr);
    ~SyncingPage();
    void addFile(QString filename, QString datasize,QString speed,QString progress);
    void removeFile();
    void updateFilenameText();
    QVBoxLayout*filesLayout;
    QWidget* filenameWidget = new QWidget();
    FileCard*FileCardArea1=new FileCard("文件1","3GB","1.3MB/s","0%");
    FileCard*FileCardArea2=new FileCard("文件2","3.55GB","2MB/s","0%");
    FileCard*FileCardArea3=new FileCard("文件3fsgfhgsgskhdjfskjfbskhbfsbkhebvsbvhisbhjevkhba","3.5GB","3MB/s","52%");
    FileCard*FileCardArea4=new FileCard("文件vnjskbdkvsdjabjkvavshjajvnhsnvgdbdbsbsnjnsb","32GB","4MB/s","0%");
    FileCard*FileCardArea5=new FileCard("文件bb","3.45GB","51MB/s","5%");
    FileCard*FileCardArea6=new FileCard("文件6","256.2KB","65MB/s","10%");
    FileCard*FileCardArea7=new FileCard("文件7vbhksfbvbfshvbkjsbvfb","21GB","7MB/s","0%");
    FileCard*FileCardArea8=new FileCard("文件8","3.5GB","8MB/s","0%");
    FileCard*FileCardArea9=new FileCard("文件9vsnjkdfbvsvb","3.5GB","9MB/s","0%");
    FileCard*FileCardArea10=new FileCard("文件10v","3.5GB","10MB/s","0%");
private:
    ElaProgressBar* _progressBar{nullptr};
    ElaPushButton* _pushButton{nullptr};
    ElaToggleButton* _toggleButton{nullptr};
protected:
    void resizeEvent(QResizeEvent* event) override;
};

#endif // SYNCING_VIEW_H
