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
        if(x.getSyncStatus()==2||x.getSyncStatus()==-1){
            continue;
        }
        QMap<QString,QString> map;
        map = cosclient->getObjectTagging(x.getRemotePath(),"");
        if(map.contains("computerName")){
            if(map["computerName"]!=computerName){
                if(map.contains("lastSyncTime")){
                    bool timeOut=false;
                    if(lastSyncTimeMap.contains(x.getRemotePath())){
                        if(lastSyncTimeMap[x.getRemotePath()].secsTo(QDateTime::currentDateTime())>12){
                            emit cloudDirectoryChanged(x,0);
                          otherDeviceMap[x.getRemotePath()]=map["computerName"];
                          lastSyncTimeMap[x.getRemotePath()]=QDateTime::currentDateTime();
                          continue;
                        }
                    }
                    else{
                        lastSyncTimeMap[x.getRemotePath()]=QDateTime::currentDateTime();
                    }
                    if(otherDeviceMap[x.getRemotePath()]!=map["computerName"]){
                        emit cloudDirectoryChanged(x,1);
                        otherDeviceMap[x.getRemotePath()]=map["computerName"];
                        lastSyncTimeMap[x.getRemotePath()]=QDateTime::currentDateTime();
                    }

                }
            }
        }
        }
    QThread::sleep(5);
    }
}
CloudListener::CloudListener(COSConfig configi) : config(configi) {
    cosclient = new COSClient(config);
};
