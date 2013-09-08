#include "protosizecheckbuilds.h"
#include "management.h"

ProtoSizeCheckBuilds::ProtoSizeCheckBuilds(QObject *parent):
        Protocol(parent)
{
}

void ProtoSizeCheckBuilds::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("BuildMD5")){
        BuildMD5(jsonObject, management, slaveSocket);
    }
}

void ProtoSizeCheckBuilds::BuildMD5(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    QString buildNo = jsonObject.value("buildNo").toString();

    QString buildMD5Value = jsonObject.value("md5Sum").toString();
    qDebug()<<"buildMD5Value="<<buildMD5Value;

    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    if(handler == 0){
        slaveSocket->disconnectFromHost();
        return;
    }

    management->slaveBuildSize(buildNo.toInt(), buildMD5Value, handler->getMachine()->getMachineID());

}

