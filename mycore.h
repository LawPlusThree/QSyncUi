#ifndef MYCORE_H
#define MYCORE_H

#include <QObject>
#include "user.h"
#include "awsutils.h"
class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = nullptr);
private:
    User* currentUser;
    AWSUtils* awsUtils;
signals:
};

#endif // MYCORE_H
