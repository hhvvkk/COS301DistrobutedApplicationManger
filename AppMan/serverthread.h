#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include "Machine.h"

//forward declaration of Management so it can be used
class Management;

class ServerThread : public QThread
{
    Q_OBJECT
public:
    explicit ServerThread(int ID,Management *m, QObject *parent = 0);
    void run();

public slots:
    void readyReadFunction();
    void disconnectedFunction();

private:
    QTcpSocket *socket;
    int socketID;
    Management *management;
    Machine *machine;
    bool firstTalk;

};

#endif // SERVERTHREAD_H
