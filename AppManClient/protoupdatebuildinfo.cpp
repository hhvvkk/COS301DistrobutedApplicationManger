#include "protoupdatebuildinfo.h"
#include "management.h"

ProtoUpdateBuildInfo::ProtoUpdateBuildInfo(QObject *parent) :
    Protocol(parent)
{
}

ProtoUpdateBuildInfo::~ProtoUpdateBuildInfo(){

}

void ProtoUpdateBuildInfo::handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("UpdateBuildName"))
        UpdateBuildName(jsonObject, man, slaveSocket);
}

void ProtoUpdateBuildInfo::UpdateBuildName(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    int buildID = jsonObject.value("buildID").toInt();
    QString newBuildName = jsonObject.value("buildName").toString();

    management->updateBuildName(buildID, newBuildName);

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoUpdateBuildInfo", true);
    appendJSONValue(jsonMessage, "subHandler", "UpdatedBuildName", true);
    appendJSONValue(jsonMessage, "buildID", QString::number(buildID), true);
    appendJSONValue(jsonMessage, "buildName", newBuildName, false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toUtf8().data());
    slaveSocket->flush();
}
