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
    Server::close();
}

void Server::setPort(int newPort){
    port = newPort;
    stopServer();
    startServer();
}

void Server::incomingConnection(int socketID){

    ServerThread *serverThread = new ServerThread(socketID, management, this);

    connect(serverThread, SIGNAL(finished()),serverThread, SLOT(deleteLater()));

    serverThread->start();
}
