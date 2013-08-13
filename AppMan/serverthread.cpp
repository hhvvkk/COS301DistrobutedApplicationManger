#include "serverthread.h"


#include "Machine.h"
#include "management.h"

ServerThread::ServerThread(int ID, Management *m, QObject *parent) :
    QThread(parent),
    management(m)
{
    this->socketID = ID;
    firstTalk = true;
}

void ServerThread::run(){
    //thread starts here
    qDebug() << socketID << " Starting thread";
    socket = new QTcpSocket();


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

void ServerThread::readyReadFunction(){

    QByteArray Data = socket->readAll();
    QString data = Data;
    qDebug()<<"read:::"<<data;
    if(firstTalk == true){
        firstTalk = false;
        //means it must be first time spoken to. Thus it requires 'Hello AppMan' or disconnect
        if(!data.compare("Hello AppMan")){
            //means a new client has connected
            QHostAddress adr = socket->peerAddress();
            qDebug()<<"New"<<adr.toString();
            socket->write("Hello AppManClient");
            machine = new Slave(1,adr.toString());
            management->addMachine(machine);
            machine->setServerThread(this);
        }
        else{
            socket->disconnectFromHost();
        }
        return;
    }else{
        //first talk is false thus other protocols follows
        qDebug()<<"FirstTalk = FALSE";
    }

}

void ServerThread::disconnectedFunction(){
    management->removeMachine(machine);
    //delete socket once it is not in use
    socket->deleteLater();
    //this exit shows that the thread is done and don't have to be on loop
    exit(0);
}
