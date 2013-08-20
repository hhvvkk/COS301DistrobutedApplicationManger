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

    buildIterator = 0;
    allBuilds = 0;
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

    if(allBuilds == 0)
        allBuilds = management->getAllBuilds();


    if(buildIterator < management->getBuildCount()){
        qDebug()<<"RecheckingWithBuild ->"<<allBuilds[buildIterator].getBuildID();
        rechecker = "Rechecker:#"+QString::number(allBuilds[buildIterator].getBuildID())+"#"+allBuilds[buildIterator].getBuildName();
        socket->write(rechecker.toAscii().data());
        socket->flush();
        buildIterator++;
    }
    else{
        //reset the iterator
        allBuilds = 0;
        buildIterator = 0;
        socket->write("RecheckDone");
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
        //this basically loops through the builds and sends it to master
        //
        this->recheckBuildsPhase2();
        return;
    }

    if(data.contains("CopyBuildOver:#")){
        this->copyBuildOver(data);
    }
    else{

    }

    if(data.contains("SizeCheckBuild:#")){
        this->sizeCheckBuild(data);
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

    xmlWriter xWrite;
    Build buildToAdd = management->createBuildDirectory(newBuild);
    xWrite.receiveBuild(QString::number(buildToAdd.getBuildID()),buildToAdd.getBuildName(),buildToAdd.getBuildDescription(),buildToAdd.getBuildDirectory());
    xWrite.CreateXMLFile();
    management->addBuild(buildToAdd);

    QString buildAddedMessage = "GotABuild:#"+buildNo;
    socket->write(buildAddedMessage.toAscii().data());
    socket->flush();//write all that should be written
}

void SocketClient::sizeCheckBuild(QString data){
    //this means it is build information that is flowing.
    //E.g MD5Inspection:#1
    QString mostLeft = "SizeCheckBuild:#";

    QString buildNo = data.right((data.size()-mostLeft.length()));

    qDebug()<<"SizeCheckBuildvv=========? "<<buildNo;

}
