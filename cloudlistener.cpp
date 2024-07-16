#include "cloudlistener.h"


void CloudListener::setTasks(const QVector<SyncTask> &tasks)
{
    myTasks=tasks;
}



CloudListener::CloudListener() {
    computerName = QSysInfo::machineHostName();
}

void CloudListener::run()
{
    while (true) {
    for (auto const &x: myTasks){
        QMap<QString,QString> map;
        map = cosclient->getObjectTagging(x.getRemotePath(),"");
        if(map.contains("computerName")){
            if(map["computerName"]!=computerName){
                if(map.contains("lastSyncTime")){
                    if(otherDeviceMap[x.getRemotePath()]==map["computerName"]){
                        continue;
                    }
                    emit cloudDirectoryChanged(x);
                    otherDeviceMap[x.getRemotePath()]=map["computerName"];
                }
            }
        }
        }
    QThread::sleep(10);
    }
}
CloudListener::CloudListener(COSConfig configi) : config(configi) {
    cosclient = new COSClient(config);
};
