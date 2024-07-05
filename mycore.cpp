#include "mycore.h"

Core::Core(QObject *parent)
    : QObject{parent}
{
    currentUser=nullptr;
    //awsUtils=new AWSUtils();
}
