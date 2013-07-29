#include "socketclient.h"

SocketClient::SocketClient(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
}

SocketClient::~SocketClient(){
    delete socket;
}



void SocketClient::connectToServer(QString ipAddress, int port){
    //making sure it is disconnectedclose();

    qDebug()<<":connecting"<<ipAddress<< "---" <<port;
    socket->connectToHost(ipAddress, port);

    //wait for one second for connection
    if(!socket->waitForConnected(1000)){
        //error
        qDebug()<<"Error when connecting";
    }

}

void SocketClient::disconnectFromServer(){
    socket->disconnectFromHost();

    delete socket;
    socket = new QTcpSocket(this);
}

void SocketClient::connected(){
    qDebug()<<"connected!;";

    //write to the server
    socket->write("Hello AppMan");
}

void SocketClient::disconnected(){
    qDebug()<<"disconected!";
}


void SocketClient::bytesWritten(qint64 bytes){
    qDebug()<<"we wrote "<<bytes<< " bytes";
}


void SocketClient::readyRead(){
    //ready to read data off socket
    qDebug()<<"reading...";
    qDebug()<< socket->readAll();

}
