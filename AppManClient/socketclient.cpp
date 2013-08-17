#include "socketclient.h"
#include "management.h"
#include "Build.h"

SocketClient::SocketClient(Management *man, QObject *parent) :
    QObject(parent),
    management(man)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));

    connect(this, SIGNAL(connectionEstablished()), this, SLOT(recheckBuildsPhase1()));
}

SocketClient::~SocketClient(){
    delete socket;
}

void SocketClient::recheckBuildsPhase1(){
    //to get an acknowledgement that the builds can be rechecked
    qDebug()<<"RecheckBuildsphase1";
    socket->write("RecheckBuilds");
    socket->flush();
}


void SocketClient::recheckBuildsPhase2(){
    QString rechecker = "";

    Build * allBuilds = management->getAllBuilds();

    for(int i = 0; i < management->getBuildCount(); i++){
        qDebug()<<"RecheckingWithBuild ->"<<allBuilds[i].getBuildID();
        rechecker = "Rechecker:#"+QString::number(allBuilds[i].getBuildID())+"#"+allBuilds[i].getBuildName();
        socket->write(rechecker.toAscii().data());
        socket->flush();
    }
}


void SocketClient::connectToServer(QString ipAddress, int port){
    //making sure it is disconnectedclose();

    qDebug()<<":connecting"<<ipAddress<< "---" <<port;
    socket->connectToHost(ipAddress, port);

    //wait for one second for connection
    if(!socket->waitForConnected(1000)){
        //error
        qDebug()<<"Error when connecting";
        return;
    }
    //write to the server
    socket->write("Hello AppMan");
    socket->flush();
}

void SocketClient::disconnectFromServer(){
    socket->disconnectFromHost();

    delete socket;
    socket = new QTcpSocket(this);

    //RECONNECT SIGNALS...
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
}

void SocketClient::connected(){
    qDebug()<<"connected!;";

}

void SocketClient::disconnected(){
    qDebug()<<"disconected!";
}


void SocketClient::bytesWritten(qint64 bytes){
    qDebug()<<"we wrote "<<bytes<< " bytes";
}


void SocketClient::readyRead(){
    //ready to read data off socket
    QByteArray Data = socket->readAll();
    QString data = Data;
    qDebug()<<"read::"<<data;

    if(!data.compare("Hello AppManClient")){
        //means a connection was established
        emit connectionEstablished();
        return;
    }

    if(!data.compare("RecheckCopy")){
        this->recheckBuildsPhase2();
        return;
    }

    if(data.contains("CopyBuildOver:#")){
        this->copyBuildOver(data);
    }
    else{

    }

}

void SocketClient::copyBuildOver(QString data){
    //E.g CopyBuildOver:#1#NameBlah
    //15 = size of 'CopyBuildOver:#'

    QString rightSide = data.right((data.size()-15));
    //E.g. RIGHT SIDE= "1#NameBlah"


    //REASONING behind this method(THE INDEX COUNTING)
    //is that once you have for instance multiple # in name...
    //this prevents problems from occurring
    QString buildNo = rightSide.left(rightSide.indexOf("#"));
    // +1 to account for # as well
    QString buildName = rightSide.right(rightSide.length() - (buildNo.length()+1));
    Build newBuild = Build(buildNo.toInt() , buildName,"", "");

    management->addBuild(newBuild);

    QString buildAddedMessage = "GotABuild:#"+buildNo;
    socket->write(buildAddedMessage.toAscii().data());
    socket->flush();//write all that should be written
}
