#include "protoslavecurrentbuilds.h"
#include "management.h"
#include "Slave.h"
#include <QDebug>

ProtoSlaveCurrentBuilds::ProtoSlaveCurrentBuilds(QObject *parent)
    :Protocol(parent)
{
}

void ProtoSlaveCurrentBuilds::handle(QString data, Management *management, QTcpSocket *slaveSocket){
    if(!data.compare("RecheckDone")){
        RecheckDone(slaveSocket);
    }
    if(data.contains("Rechecker")){
        Rechecker(data, management, slaveSocket);
    }
    if(!data.compare("RecheckBuilds")){
        RecheckBuilds(slaveSocket);
    }
}

void ProtoSlaveCurrentBuilds::RecheckDone(QTcpSocket *slaveSocket){
    //at this point of time the rechecking is done and all that machines builds
    //are placed inside view, It only needs to check all build sizes
    SizeCheckAllBuilds(slaveSocket);
}


void ProtoSlaveCurrentBuilds::SizeCheckAllBuilds(QTcpSocket *slaveSocket){
    slaveSocket->write("||SizeCheckAllBuilds||");
    slaveSocket->flush();
}

void ProtoSlaveCurrentBuilds::Rechecker(QString data, Management *management, QTcpSocket *socket){
    //this means it is build information that is following.
    //E.g Rechecker:#1#NameBlah
    QString mostLeft = "Rechecker:#";

    QString rightSide = data.right((data.size()-mostLeft.length()));
    //E.g. RIGHT SIDE= "1#NameBlah"
    //REASONING behind this method(THE INDEX COUNTING)
    //is that once you have for instance multiple # in name...
    //this prevents problems from occurring
    QString buildNo = rightSide.left(rightSide.indexOf("#"));

    //QString buildName = rightSide.right(rightSide.length() - (buildNo.length()+1));

    QHostAddress adr = socket->peerAddress();
    QString slaveIp = adr.toString();

    management->addBuildToSlave(slaveIp, buildNo);

    socket->write("||RecheckCopy||");
    socket->flush();
}

void ProtoSlaveCurrentBuilds::RecheckBuilds(QTcpSocket *slaveSocket){
    //this is just to acknowledge that the client can recheck all builds
    slaveSocket->write("||RecheckCopy||");
    slaveSocket->flush();
}
