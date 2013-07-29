#include "network.h"

Network::Network()
{
    socketClient = new SocketClient();
}

void Network::setConnectionDetails(QString ip, int port){
    this->IpAddress = ip;
    this->serverPort = port;
}

void Network::connecToServer(){
    socketClient->connectToServer(this->IpAddress, this->serverPort);
}

void Network::disconnect(){
    socketClient->disconnectFromServer();
}
