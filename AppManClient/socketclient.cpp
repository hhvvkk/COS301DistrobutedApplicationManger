#include "socketclient.h"
#include "management.h"
#include "Build.h"

SocketClient::SocketClient(ProtocolHandler * protoHandler, QTcpSocket * newSocket, QObject *parent) :
    QObject(parent),
    socket(newSocket),
    protocolHandler(protoHandler)
{
}

SocketClient::~SocketClient(){
    socket->deleteLater();
}


void SocketClient::disconnected(){
    protocolHandler->disconnectFromMaster();
    this->deleteLater();
}

void SocketClient::readyRead(){
    //ready to read data off socket
    if(socket == 0){
        return;
    }
    QByteArray Data = socket->readAll();
    QString data = Data;

    protocolHandler->handle(data);
}
