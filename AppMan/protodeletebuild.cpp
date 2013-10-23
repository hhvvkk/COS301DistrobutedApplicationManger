#include "protodeletebuild.h"
#include "protocolhandler.h"
#include "Slave.h"
#include "management.h"

ProtoDeleteBuild::ProtoDeleteBuild(QObject *parent)
    :Protocol(parent)
{
}


ProtoDeleteBuild::~ProtoDeleteBuild(){

}

void ProtoDeleteBuild::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("DeleteSuccess"))
        DeleteSuccess(jsonObject, management);
}


void ProtoDeleteBuild::deleteBuildFromSlave(QTcpSocket *slaveSocket, int buildID){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoDeleteBuild", true);
    appendJSONValue(jsonMessage, "subHandler", "DeleteBuild", true);
    appendJSONValue(jsonMessage, "buildID", QString::number(buildID), false);

    endJSONMessage(jsonMessage);
    sendJSONMessage(slaveSocket, jsonMessage);
}

void ProtoDeleteBuild::DeleteSuccess(QVariantMap jsonObject, Management *management){

    bool ok = false;

    int buildID = jsonObject.value("buildID").toInt(&ok);

    if(!ok)
        return;

    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    if(handler == 0){
        return;
    }

    int machineID = -1;

    machineID = handler->getMachine()->getMachineID();


    QtConcurrent::run(management, &Management::slaveSuccessDeleteABuild, machineID, buildID);
}
