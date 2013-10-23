#include "protodeletebuild.h"
#include "management.h"

ProtoDeleteBuild::ProtoDeleteBuild(QObject *parent)
    :Protocol(parent)
{
}


ProtoDeleteBuild::~ProtoDeleteBuild(){

}

void ProtoDeleteBuild::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("DeleteBuild"))
        DeleteBuild(slaveSocket, jsonObject, management);
}


void ProtoDeleteBuild::DeleteBuild(QTcpSocket *slaveSocket, QVariantMap jsonObject, Management *management){
    bool ok = false;

    //get the ID to be deleted
    int buildID = jsonObject.value("buildID").toInt(&ok);

    if(!ok)
        return;

    management->deleteBuild(buildID);


    //NOTIFY The sucessful delete
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoDeleteBuild", true);
    appendJSONValue(jsonMessage, "subHandler", "DeleteSuccess", true);
    appendJSONValue(jsonMessage, "buildID", QString::number(buildID), false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toUtf8().data());
    slaveSocket->flush();
}
