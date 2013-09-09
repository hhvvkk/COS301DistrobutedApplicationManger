#include "serverthread.h"
#include "Machine.h"
#include "management.h"

ServerThread::ServerThread(int ID, Management *man, QObject *parent)
    :QThread(parent)
{
    this->socketID = ID;
    socket = 0;

    //important to create the socket for ProtocolHandler to use
    //socket is set in run...
    protocolHandler = new ProtocolHandler(man, this);
}

//thread starts here
void ServerThread::run(){
    socket = new QTcpSocket();

    protocolHandler->setSocket(socket);

    if(!socket->setSocketDescriptor(this->socketID)){
        //qDebug()<<"socketID can't be set";
    }

    //connect the function that will execute when there are things waiting to be read
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyReadFunction()),Qt::DirectConnection);

    //connect the disconnected which will execute once client has disconnected
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectedFunction()),Qt::DirectConnection);

    //NB exec() is needed!!!
    //this execute keeps the thread running in a loop thus it won't exit once it is done
    exec();
}

QTcpSocket *ServerThread::getSocket(){
    return socket;
}

void ServerThread::readyReadFunction(){
    QByteArray Data = socket->readAll();
    QString data = Data;

    //Send the data to the requestHandler to handle
    protocolHandler->handle(data);
}

void ServerThread::disconnectedFunction(){
    //QString machineToDisconnect = socket->peerAddress().toString();
    protocolHandler->disconnectMachine();

    socket->deleteLater();
    protocolHandler->deleteLater();

    //this exit shows that the thread is done and don't have to be on loop
    exit(0);
}
