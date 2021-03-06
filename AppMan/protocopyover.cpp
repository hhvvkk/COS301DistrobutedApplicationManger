#include "protocopyover.h"
#include "management.h"
#include <QHostAddress>

ProtoCopyOver::ProtoCopyOver(QObject *parent) :
    Protocol(parent)
{
}


ProtoCopyOver::~ProtoCopyOver(){

}

void ProtoCopyOver::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("GotABuild"))
        GotABuild(jsonObject, management, slaveSocket);
}

void ProtoCopyOver::GotABuild(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){

    QString BuildID = jsonObject.value("BuildID").toString();

    bool ok = false;

    int BuildIDId = BuildID.toInt(&ok);

    if(!ok)
        return;

    QString buildName = management->getBuildByID(BuildIDId)->getBuildName();

    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    management->addBuildToSlave(handler->getMachine()->getMachineID(), BuildIDId, buildName);

    //finally call size check on that build
    sizeCheckCertainBuild(BuildID, slaveSocket);
}


void ProtoCopyOver::copyBuildOver(int buildId, QString buildName, QTcpSocket *slaveSocket){
    QString buildIdString = QString::number(buildId);

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoCopyOver",true);
    appendJSONValue(jsonMessage,"subHandler","CopyBuildOver",true);
    appendJSONValue(jsonMessage, "BuildID", buildIdString,true);
    appendJSONValue(jsonMessage, "buildName", buildName,false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}

void ProtoCopyOver::sizeCheckCertainBuild(QString BuildID, QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoSizeCheckBuilds",true);
    appendJSONValue(jsonMessage,"subHandler","SizeCheckABuild",true);
    appendJSONValue(jsonMessage, "BuildID", BuildID,false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}
