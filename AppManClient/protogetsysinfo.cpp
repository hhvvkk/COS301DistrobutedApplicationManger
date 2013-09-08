#include "protogetsysinfo.h"
#include "management.h"

ProtoGetSysInfo::ProtoGetSysInfo(QObject *parent)
    :Protocol(parent)
{
}

void ProtoGetSysInfo::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket) {
    if(!jsonObject.value("subHandler").toString().compare("GetDetailedSysInfo"))
        GetDetSysInfo(management, masterSocket);
    if(!jsonObject.value("subHandler").toString().compare("GetMinimalSysInfo"))
        GetMinSysInfo(management, masterSocket);
}

void ProtoGetSysInfo::GetDetSysInfo(Management *management, QTcpSocket *masterSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoGetSysInfo",true);
    appendJSONValue(jsonMessage,"subHandler","DetailedSysInfoFollows",true);
    appendJSONValue(jsonMessage, "data", management->getDetSysInfo(),false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toAscii().data());
    masterSocket->flush();
}

void ProtoGetSysInfo::GetMinSysInfo(Management *management, QTcpSocket *masterSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoGetSysInfo",true);
    appendJSONValue(jsonMessage,"subHandler","MinimalSysInfoFollows",true);
    appendJSONValue(jsonMessage, "data", management->getMinSysInfo(),false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toAscii().data());
    masterSocket->flush();
}
