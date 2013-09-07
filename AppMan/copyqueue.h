#ifndef COPYQUEUE_H
#define COPYQUEUE_H

#include <QObject>

class CopyQueue : public QObject
{
    Q_OBJECT
public:
    explicit CopyQueue(QObject *parent = 0);

signals:

public slots:

};

#endif // COPYQUEUE_H
