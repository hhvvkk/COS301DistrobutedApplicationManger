#ifndef SIMTHREAD_H
#define SIMTHREAD_H

#include <QObject>
#include <QThread>
#include <QMap>
#include <QMapIterator>

#include "management.h"

class simThread : public QThread
{
    Q_OBJECT

public:
    explicit simThread(QObject *parent = 0,QString build = "", QString recArg = "", Management * man = 0);
    void run();
    ~simThread();
private:
    QString stBuild;
    QString stArg;
    Management *management;
    
};

#endif // SIMTHREAD_H
