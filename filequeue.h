#ifndef FILEQUEUE_H
#define FILEQUEUE_H

#include <QObject>

class fileQueue : public QObject
{
    Q_OBJECT
public:
    explicit fileQueue(QObject *parent = nullptr);

signals:
};

#endif // FILEQUEUE_H
