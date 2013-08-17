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
            machine = new Slave(1,adr.toString());
            management->addMachine(machine);
            machine->setServerThread(this);

            socket->write("Hello AppManClient");
            socket->flush();
        }
        else{
            socket->disconnectFromHost();
        }
        return;
    }

    //else it is other commands
    if(data.contains("GotABuild:#")){
            GotABuild(data);
            return;
    }

    if(!data.compare("RecheckBuilds")){
        RecheckBuilds(data);
    }else if(!data.compare("RecheckDone")){
        //at this point of time the rechecking is done and all that machines builds
        //can be placed inside the view
        qDebug()<<"RECHECK___DONE";
    }
    else if(data.contains("Rechecker:#")){
        Rechecker(data);
    }

}

void ServerThread::disconnectedFunction(){
    management->removeMachine(machine);
    //delete socket once it is not in use
    socket->deleteLater();
    //this exit shows that the thread is done and don't have to be on loop
    exit(0);
}


/*
 * NEW------
 */

void ServerThread::copyBuildOver(int buildId, QString buildName){
    QString buildIdString = QString::number(buildId);
    QString writeBuildMessage = "CopyBuildOver:#"+ buildIdString +"#"+buildName;
    socket->write(writeBuildMessage.toAscii().data());
    socket->flush();//write all that should be written
}

void ServerThread::GotABuild(QString data){
    //at this point of time in communication, a build has just been added
    //because 1. it was already there or 2. it has just been added
    //"GotABuild:#"+buildNo;

    QHostAddress adr = socket->peerAddress();

    QString leftSide = data.left(data.indexOf("#"));
    QString buildNo = data.right(data.length() - (leftSide.length()+1));
    QString slaveIp = adr.toString();
    management->addBuildToSlave(slaveIp, buildNo);
}

void ServerThread::RecheckBuilds(QString data){
    //this is just to acknowledge that the client can recheck all builds
    socket->write("RecheckCopy");
    socket->flush();
    qDebug()<<"RECHECKCOPY";
}


void ServerThread::RecheckDone(QString data){

}

void ServerThread::Rechecker(QString data){
    //this means it is build information that is flowing.
    //E.g Rechecker:#1#NameBlah
    QString mostLeft = "Rechecker:#";

    QString rightSide = data.right((data.size()-mostLeft.length()));
    //E.g. RIGHT SIDE= "1#NameBlah"
    //REASONING behind this method(THE INDEX COUNTING)
    //is that once you have for instance multiple # in name...
    //this prevents problems from occurring
    QString buildNo = rightSide.left(rightSide.indexOf("#"));

    QString buildName = rightSide.right(rightSide.length() - (buildNo.length()+1));
    qDebug()<<"BuildNO::::"<<buildNo;
    qDebug()<<"BuildNa::::"<<buildName;
    QHostAddress adr = socket->peerAddress();
    QString slaveIp = adr.toString();

    management->addBuildToSlave(slaveIp, buildNo);
}
