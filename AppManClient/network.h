#ifndef NETWORK_H
#define NETWORK_H

#include <QString>
#include "socketclient.h"

class Network
{
public:
    Network();
    void connecToServer();
    void setConnectionDetails(QString ip, int port);
    void disconnect();
    void writeSOme(){
        socketClient->writeSome();
    }

private:
    SocketClient *socketClient;
    int serverPort;
    QString IpAddress;
};

#endif // NETWORK_H
