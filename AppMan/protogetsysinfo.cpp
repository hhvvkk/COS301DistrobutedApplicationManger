#include "protogetsysinfo.h"
#include "management.h"

ProtoGetSysInfo::ProtoGetSysInfo(QObject *parent)
    :Protocol(parent)
{
}

ProtoGetSysInfo::~ProtoGetSysInfo(){

}

void ProtoGetSysInfo::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("DetailedSysInfoFollows"))
        DetailedSysInfoFollows(jsonObject, management);
    if(!jsonObject.value("subHandler").toString().compare("MinimalSysInfoFollows"))
        MinimalSysInfoFollows(jsonObject, management);
}

void ProtoGetSysInfo::DetailedSysInfoFollows(QVariantMap jsonObject, Management * management){
    QString data = jsonObject.value("data").toString();
    int machineID = jsonObject.value("id").toInt();
    Machine* m = management->getMachineById(machineID);
    m->setDetStatsString(data);
}

void ProtoGetSysInfo::MinimalSysInfoFollows(QVariantMap jsonObject, Management *management){
    QString data = jsonObject.value("data").toString();        
    int machineID = jsonObject.value("id").toInt();
    Machine* m = management->getMachineById(machineID);
    m->setMinStatsString(data);
}

void ProtoGetSysInfo::getDetailed(QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoGetSysInfo",true);
    appendJSONValue(jsonMessage,"subHandler","GetDetailedSysInfo",false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}

void ProtoGetSysInfo::getMinimal(QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoGetSysInfo",true);
    appendJSONValue(jsonMessage,"subHandler","GetMinimalSysInfo",false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}
