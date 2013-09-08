#include "protoslavecurrentbuilds.h"
#include "management.h"

ProtoSlaveCurrentBuilds::ProtoSlaveCurrentBuilds(QObject *parent)
    :Protocol(parent)
{
    buildIterator = 0;
    allBuilds = 0;
}

void ProtoSlaveCurrentBuilds::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){
    if(!jsonObject.value("subHandler").toString().compare("Rechecker"))
        Rechecker(management, masterSocket);
}

void ProtoSlaveCurrentBuilds::Rechecker(Management *management, QTcpSocket *masterSocket){

    if(allBuilds == 0)
        allBuilds = management->getAllBuilds();

    if(buildIterator < management->getBuildCount()){
        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
        appendJSONValue(jsonMessage, "subHandler", "Rechecker", true);
        appendJSONValue(jsonMessage, "buildNo", QString::number(allBuilds[buildIterator].getBuildID()), true);
        appendJSONValue(jsonMessage, "buildName", allBuilds[buildIterator].getBuildName(), false);
        endJSONMessage(jsonMessage);

        masterSocket->write(jsonMessage.toAscii().data());
        masterSocket->flush();
        buildIterator++;
    }
    else{
        //reset the iterator
        allBuilds = 0;
        buildIterator = 0;


        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
        appendJSONValue(jsonMessage, "subHandler", "RecheckDone", false);
        endJSONMessage(jsonMessage);

        masterSocket->write(jsonMessage.toAscii().data());
        masterSocket->flush();
    }
}
