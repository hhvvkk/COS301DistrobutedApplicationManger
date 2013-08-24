#include "protoconnect.h"
#include "protocolhandler.h"
#include "Slave.h"
#include "management.h"

ProtoConnect::ProtoConnect(QObject *parent)
    :Protocol(parent)
{
}

void ProtoConnect::handle(QString data, Management *management, QTcpSocket *slaveSocket){

    //means it must be first time spoken to. Thus it requires 'Hello AppMan' or disconnect
    if(!data.compare("Hello AppMan")){
        //means a new client has connected
        QHostAddress address = slaveSocket->peerAddress();
        QString slaveAddressIp = address.toString();
        //the parent is set as the protocol handler! Thus get the handler by using parent() function

        QObject *myParent = this->parent();
        if(myParent == 0)
            return;

        ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

        if(handler == 0){
            slaveSocket->disconnectFromHost();
            return;
        }

        management->addMachine(slaveAddressIp, handler);

        slaveSocket->write("||Hello AppManClient||");
        slaveSocket->flush();
    }
    else{
        slaveSocket->disconnectFromHost();
    }
    return;
}


void ProtoConnect::disconnectMachine(Machine *machine, Management *management){
    //remove the machine from management
    management->removeMachine(machine);
}
