#include "protocopyover.h"
#include "management.h"
#include <QHostAddress>

ProtoCopyOver::ProtoCopyOver(QObject *parent) :
    Protocol(parent)
{
}

void ProtoCopyOver::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("GotABuild"))
        GotABuild(jsonObject, management, slaveSocket);
}

void ProtoCopyOver::GotABuild(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){

    QString buildNo = jsonObject.value("buildNo").toString();

    int buildNoId = buildNo.toInt();
    QString buildName = management->getBuildByID(buildNoId).getBuildName();

    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    management->addBuildToSlave(handler->getMachine()->getMachineID(), buildNoId, buildName);

    //finally call size check on that build
    sizeCheckCertainBuild(buildNo, slaveSocket);
}


void ProtoCopyOver::copyBuildOver(int buildId, QString buildName, QTcpSocket *slaveSocket){
    QString buildIdString = QString::number(buildId);

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoCopyOver",true);
    appendJSONValue(jsonMessage,"subHandler","CopyBuildOver",true);
    appendJSONValue(jsonMessage, "buildNo", buildIdString,true);
    appendJSONValue(jsonMessage, "buildName", buildName,false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toAscii().data());
    slaveSocket->flush();//write all that should be written
}

void ProtoCopyOver::sizeCheckCertainBuild(QString buildNo, QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoSizeCheckBuilds",true);
    appendJSONValue(jsonMessage,"subHandler","SizeCheckABuild",true);
    appendJSONValue(jsonMessage, "buildNo", buildNo,false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toAscii().data());
    slaveSocket->flush();
}
