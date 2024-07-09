#ifndef MESSAGECHANNEL_H
#define MESSAGECHANNEL_H

#include <QObject>

class MessageChannel : public QObject
{
    Q_OBJECT
public:
    explicit MessageChannel(QObject *parent = nullptr);
signals:
    void message( QString message, QString type);
};

#endif // MESSAGECHANNEL_H
