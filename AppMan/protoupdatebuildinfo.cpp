#include "protoupdatebuildinfo.h"
#include "management.h"
#include "Slave.h"

ProtoUpdateBuildInfo::ProtoUpdateBuildInfo(QObject *parent) :
    Protocol(parent)
{
}

ProtoUpdateBuildInfo::~ProtoUpdateBuildInfo(){

}

void ProtoUpdateBuildInfo::handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("UpdatedBuildName"))
        UpdatedBuildName(jsonObject, man, slaveSocket);
}

void ProtoUpdateBuildInfo::updateName(int buildID, QString newBuildName, QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoUpdateBuildInfo", true);
    appendJSONValue(jsonMessage, "subHandler", "UpdateBuildName", true);
    appendJSONValue(jsonMessage, "buildID", QString::number(buildID), true);
    appendJSONValue(jsonMessage, "buildName", newBuildName, false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toUtf8().data());
    slaveSocket->flush();
}

void ProtoUpdateBuildInfo::UpdatedBuildName(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    QString buildName = jsonObject.value("buildName").toString();
    int buildID = jsonObject.value("buildID").toInt();

    QObject *myParent = this->parent();

    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    if(handler == 0){
        return;
    }

    Machine *thisMachine = handler->getMachine();

    if(thisMachine == 0)
        return;


    management->slaveUpdatedBuildName(thisMachine->getMachineID(), buildID, buildName);
}


