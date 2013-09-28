#include "protosizecheckbuilds.h"
#include "management.h"

ProtoSizeCheckBuilds::ProtoSizeCheckBuilds(QObject *parent):
        Protocol(parent)
{
}

ProtoSizeCheckBuilds::~ProtoSizeCheckBuilds(){

}

void ProtoSizeCheckBuilds::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("BuildMD5")){
        BuildMD5(jsonObject, management, slaveSocket);
    }
}

void ProtoSizeCheckBuilds::BuildMD5(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    QString BuildID = jsonObject.value("BuildID").toString();

    QString OneBuildOnly = jsonObject.value("OneBuildOnly").toString();

    QString buildMD5Value = jsonObject.value("md5Sum").toString();

    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    if(handler == 0){
        slaveSocket->disconnectFromHost();
        return;
    }

    if(OneBuildOnly.compare("true")){
        management->slaveBuildSize(BuildID.toInt(), buildMD5Value, handler->getMachine()->getMachineID());
    }
    else{
        management->slaveABuildSize(BuildID.toInt(), buildMD5Value, handler->getMachine()->getMachineID());
    }


}

void ProtoSizeCheckBuilds::invokeSizeCheckAll(QTcpSocket *slaveSocket){
    /*The following is to create a loop to continually update
      the build until the build is fully synched
      */
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "SizeCheckAllBuilds", false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toAscii().data());
    slaveSocket->flush();
}

