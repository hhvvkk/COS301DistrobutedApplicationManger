#include "network.h"
#include "management.h"

Network::Network(Management *man)
{
    protocolHandler = new ProtocolHandler(man);
}

Network::~Network(){
    protocolHandler->deleteLater();
}

void Network::setConnectionDetails(QString ip, int port){
    this->IpAddress = ip;
    this->serverPort = port;
}

void Network::connecToServer(){
    //socketClient->connectToServer(this->IpAddress, this->serverPort);
    protocolHandler->connectToServer(this->IpAddress, this->serverPort);
}

void Network::disconnect(){
    protocolHandler->disconnectFromMaster();
}
