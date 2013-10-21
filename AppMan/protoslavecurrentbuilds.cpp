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
    /*The following is to create a loop to continually update
      the build until the information is updated
      **Same is done in the bottom of protosizecheckallbuilds
      */
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "SizeCheckAllBuilds", false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}

void ProtoSlaveCurrentBuilds::Rechecker(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){

    QString BuildID = jsonObject.value("BuildID").toString();

    QString buildName = jsonObject.value("buildName").toString();

    int BuildIDId = BuildID.toInt();

    QtConcurrent::run(this, &ProtoSlaveCurrentBuilds::addBuildToSlave, management, BuildIDId, buildName);

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "Rechecker", false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}

void ProtoSlaveCurrentBuilds::addBuildToSlave(Management *management, int buildID, QString buildName){
    if(management == 0)
        return;

    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    if(handler == 0){
        return;
    }

    management->addBuildToSlave(handler->getMachine()->getMachineID(), buildID, buildName);
}

void ProtoSlaveCurrentBuilds::RecheckBuilds(QTcpSocket *slaveSocket){
    //this is just to acknowledge that the client can recheck all builds

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "Rechecker", false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toUtf8().data());
    slaveSocket->flush();
}
