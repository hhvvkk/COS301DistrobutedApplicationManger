#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QDebug>
#include "serverthread.h"
#include "management.h"

//forward declaration so that it can be used
class Management;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(Management *m, QObject *parent = 0);
    void startServer();
    void stopServer();
    void setPort(int newPort);

protected:
    void incomingConnection(int socketID);
private:
    Management *management;
    int port;

};

#endif // SERVER_H
