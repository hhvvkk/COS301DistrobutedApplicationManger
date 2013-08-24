#include "socketclient.h"
#include "management.h"
#include "Build.h"

SocketClient::SocketClient(Management *man, QObject *parent) :
    QObject(parent),
    management(man)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));

    protocolHandler = new ProtocolHandler(management, this);
}

SocketClient::~SocketClient(){
    delete socket;
}


void SocketClient::connectToServer(QString ipAddress, int port){
    //making sure it is disconnectedclose();

    socket->connectToHost(ipAddress, port);

    //wait for one second for connection
    if(!socket->waitForConnected(1000)){
        //error
        qDebug()<<"Error when connecting";
        return;
    }
    //write to the server
    socket->write("||Hello AppMan||");
    socket->flush();
}

void SocketClient::disconnectFromServer(){
    socket->disconnectFromHost();

    delete socket;
    socket = new QTcpSocket(this);

    //RECONNECT SIGNALS...
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
}

void SocketClient::disconnected(){
    protocolHandler->disconnectedFromMaster();
}

void SocketClient::readyRead(){
    //ready to read data off socket
    QByteArray Data = socket->readAll();
    QString data = Data;
    qDebug()<<"read::"<<data;

    protocolHandler->handle(data, socket);
}
