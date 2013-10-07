#include "protoupdatemachineinfo.h"
#include "management.h"

ProtoUpdateMachineInfo::ProtoUpdateMachineInfo(QObject *parent) :
    Protocol(parent)
{
}



ProtoUpdateMachineInfo::~ProtoUpdateMachineInfo(){

}

void ProtoUpdateMachineInfo::handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){

    /*
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoUpdateMachineInfo", true);
    appendJSONValue(jsonMessage, "subHandler", "UpdateMachineID", true);
    appendJSONValue(jsonMessage, "uniqueID", QString::number(uniqueID), false);
    endJSONMessage(jsonMessage);
*/

    if(!jsonObject.value("subHandler").toString().compare("UpdateMachineID"))
        UpdateMachineID(jsonObject, man, slaveSocket);
}

void ProtoUpdateMachineInfo::UpdateMachineID(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){
    bool ok = true;

    int uniqueID = jsonObject.value("uniqueID").toInt(&ok);

    if(!ok)
        return;

    man->updateUniqueID(uniqueID);

}
