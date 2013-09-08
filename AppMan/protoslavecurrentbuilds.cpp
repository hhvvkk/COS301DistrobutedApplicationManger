#include "protoslavecurrentbuilds.h"
#include "management.h"
#include "Slave.h"
#include <QDebug>

ProtoSlaveCurrentBuilds::ProtoSlaveCurrentBuilds(QObject *parent)
    :Protocol(parent)
{
}

void ProtoSlaveCurrentBuilds::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){

    QVariant subHandler = jsonObject.value("subHandler");

    if(!subHandler.toString().compare("RecheckDone")){
        RecheckDone(slaveSocket);
    }

    if(!subHandler.toString().compare("Rechecker")){
        Rechecker(jsonObject, management, slaveSocket);
    }

    if(!subHandler.toString().compare("RecheckBuilds")){
        RecheckBuilds(slaveSocket);
    }
}

void ProtoSlaveCurrentBuilds::RecheckDone(QTcpSocket *slaveSocket){
    //at this point of time the rechecking is done and all that machines builds
    //are placed inside view, It only needs to check all build sizes
    SizeCheckAllBuilds(slaveSocket);
}


void ProtoSlaveCurrentBuilds::SizeCheckAllBuilds(QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "SizeCheckAllBuilds", false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toAscii().data());
    slaveSocket->flush();
}

void ProtoSlaveCurrentBuilds::Rechecker(QVariantMap jsonObject, Management *management, QTcpSocket *socket){

    QString buildNo = jsonObject.value("buildNo").toString();

    QString buildName = jsonObject.value("buildName").toString();

    int buildNoId = buildNo.toInt();

    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    management->addBuildToSlave(handler->getMachine()->getMachineID(), buildNoId, buildName);

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "Rechecker", false);
    endJSONMessage(jsonMessage);

    socket->write(jsonMessage.toAscii().data());
    socket->flush();
}

void ProtoSlaveCurrentBuilds::RecheckBuilds(QTcpSocket *slaveSocket){
    //this is just to acknowledge that the client can recheck all builds

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "Rechecker", false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toAscii().data());
    slaveSocket->flush();
}
