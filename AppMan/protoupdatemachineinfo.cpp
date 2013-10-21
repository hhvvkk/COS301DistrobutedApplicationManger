#include "protoupdatemachineinfo.h"

ProtoUpdateMachineInfo::ProtoUpdateMachineInfo(QObject *parent) :
    Protocol(parent)
{
}



ProtoUpdateMachineInfo::~ProtoUpdateMachineInfo(){

}

void ProtoUpdateMachineInfo::handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){

}


void ProtoUpdateMachineInfo::updateUniqueID(int uniqueID, QTcpSocket *slaveSocket){

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoUpdateMachineInfo", true);
    appendJSONValue(jsonMessage, "subHandler", "UpdateMachineID", true);
    appendJSONValue(jsonMessage, "uniqueID", QString::number(uniqueID), false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toUtf8().data());
    slaveSocket->flush();
}
