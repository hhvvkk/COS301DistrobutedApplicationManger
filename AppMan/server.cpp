#include "server.h"

Server::Server(Management *m,QObject *parent) :
    QTcpServer(parent),
    management(m),
    port(4450)
{

}

void Server::startServer(){
    if(!this->listen(QHostAddress::Any,port)){
        qDebug() << "Could not start server";
    }
    else{
        qDebug() << "Listening..."<<port;
    }
}


void Server::stopServer(){

}

void Server::setPort(int newPort){
    port = newPort;
}

void Server::incomingConnection(int socketID){
    qDebug() << socketID << " Connecting...";

    ServerThread *serverThread = new ServerThread(socketID, management, this);

    connect(serverThread, SIGNAL(finished()),serverThread, SLOT(deleteLater()));

    serverThread->start();
}
