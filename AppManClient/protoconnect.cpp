#include "protoconnect.h"
#include "management.h"

ProtoConnect::ProtoConnect(QObject *parent)
    :Protocol(parent)
{
}

void ProtoConnect::handle(QString data, Management *management, QTcpSocket *masterSocket){
    if(!data.compare("Hello AppManClient")){
        //means a connection was established
        initiateSlaveCurrentBuilds(masterSocket);
        management->setConnected(true);
        return;
    }
}

void ProtoConnect::disconnectedFromMaster(Management *management){
    management->setConnected(false);
}

void ProtoConnect::initiateSlaveCurrentBuilds(QTcpSocket *masterSocket){
    //to get an acknowledgement that the builds can be rechecked
    masterSocket->write("||RecheckBuilds||");
    masterSocket->flush();
}
