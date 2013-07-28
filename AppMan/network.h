#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include "server.h"

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = 0);
    ~Network();
    void startServer();
    void stopServer();
    void setPort(int newPort);

private:
    Server *server;
    int port;

};

#endif // NETWORK_H
