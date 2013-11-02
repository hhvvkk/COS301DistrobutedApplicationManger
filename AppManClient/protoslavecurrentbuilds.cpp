#include "protoslavecurrentbuilds.h"
#include "management.h"

ProtoSlaveCurrentBuilds::ProtoSlaveCurrentBuilds(QObject *parent)
    :Protocol(parent)
{
}

void ProtoSlaveCurrentBuilds::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){
    if(!jsonObject.value("subHandler").toString().compare("Rechecker"))
        Rechecker(management, masterSocket);
}

void ProtoSlaveCurrentBuilds::Rechecker(Management *management, QTcpSocket *masterSocket){

    for(int i = 0; i < management->getBuildCount(); i++){
        Build *build = management->getBuildAt(i);

        if(build == 0){
            return;
        }

        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
        appendJSONValue(jsonMessage, "subHandler", "Rechecker", true);
        appendJSONValue(jsonMessage, "BuildID", QString::number(build->getBuildID()), true);
        appendJSONValue(jsonMessage, "buildName", build->getBuildName(), false);
        endJSONMessage(jsonMessage);

        masterSocket->write(jsonMessage.toUtf8().data());
        masterSocket->flush();
    }

    //notify that it is done...
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "RecheckDone", false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toUtf8().data());
    masterSocket->flush();
}
