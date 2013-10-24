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
    QSettings setting("settings.ini",QSettings::IniFormat);
    setting.beginGroup("Connection");
    QString machineID = setting.value("machineID").toString();
    setting.endGroup();

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoGetSysInfo",true);
    appendJSONValue(jsonMessage,"subHandler","DetailedSysInfoFollows",true);
    appendJSONValue(jsonMessage, "data", management->getDetSysInfo(),true);
    appendJSONValue(jsonMessage, "id", machineID,false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toUtf8().data());
    masterSocket->flush();
}

void ProtoGetSysInfo::GetMinSysInfo(Management *management, QTcpSocket *masterSocket){
    QSettings setting("settings.ini",QSettings::IniFormat);
    setting.beginGroup("Connection");
    QString machineID = setting.value("machineID").toString();
    setting.endGroup();

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoGetSysInfo",true);
    appendJSONValue(jsonMessage,"subHandler","MinimalSysInfoFollows",true);
    appendJSONValue(jsonMessage, "data", management->getMinSysInfo(),true);
    appendJSONValue(jsonMessage, "id", machineID,false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toUtf8().data());
    masterSocket->flush();
}
