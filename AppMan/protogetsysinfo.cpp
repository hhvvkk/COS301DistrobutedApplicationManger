#include "protogetsysinfo.h"

ProtoGetSysInfo::ProtoGetSysInfo(QObject *parent)
    :Protocol(parent)
{
}

void ProtoGetSysInfo::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("DetailedSysInfoFollows"))
        DetailedSysInfoFollows(jsonObject, management);
    if(!jsonObject.value("subHandler").toString().compare("MinimalSysInfoFollows"))
        MinimalSysInfoFollows(jsonObject, management);
}

void ProtoGetSysInfo::DetailedSysInfoFollows(QVariantMap jsonObject, Management * management){
    QString data = jsonObject.value("data").toString();
    //emit setDetStats(stripped);
}

void ProtoGetSysInfo::MinimalSysInfoFollows(QVariantMap jsonObject, Management *management){
    QString data = jsonObject.value("data").toString();
    //emit setMinStats(stripped);
}

void ProtoGetSysInfo::getDetailed(QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoGetSysInfo",true);
    appendJSONValue(jsonMessage,"subHandler","GetDetailedSysInfo",false);
    endJSONMessage(jsonMessage);


    slaveSocket->write(jsonMessage.toAscii().data());
    slaveSocket->flush();
}

void ProtoGetSysInfo::getMinimal(QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoGetSysInfo",true);
    appendJSONValue(jsonMessage,"subHandler","GetMinimalSysInfo",false);
    endJSONMessage(jsonMessage);


    slaveSocket->write(jsonMessage.toAscii().data());
    slaveSocket->flush();
}
