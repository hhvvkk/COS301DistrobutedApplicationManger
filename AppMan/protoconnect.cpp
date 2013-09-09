#include "protoconnect.h"
#include "protocolhandler.h"
#include "Slave.h"
#include "management.h"

ProtoConnect::ProtoConnect(QObject *parent)
    :Protocol(parent)
{
}

void ProtoConnect::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){

    QVariant handler = jsonObject.value("handler");
    //means it must be first time spoken to. Thus it requires 'Hello AppMan' or disconnect
    if(handler.toString().compare("QVariant(, )")){
        QVariant subHandler = jsonObject.value("subHandler");
        if(subHandler.toString().compare("Hello AppMan")){
            //means the wrong connection is sent
            slaveSocket->disconnectFromHost();
            return;
        }

        //means a new client has connected
        //and handler value exist
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

        QString uniqueId = jsonObject.value("machineID").toString();

        if(!uniqueId.compare("QVariant(, )")){
            //means the machine returned a uniqueID as empty
            management->addMachine(-1, slaveAddressIp, handler);
            //The machine does not have a unique id, thus will go get it and write uid to slave
        }
        else{
            int thUId = uniqueId.toInt();
            management->addMachine(thUId, slaveAddressIp, handler);
        }

        Machine *myMachineObject = handler->getMachine();

        if(myMachineObject==0){
            //qDebug()<<"MY machine obj is 0--ProtoConnect";
            return;
        }

        ///DO SOMETHING WITH Unique ID........
        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage,"handler","ProtoConnect",true);
        appendJSONValue(jsonMessage,"subHandler","Hello AppManClient",true);
        appendJSONValue(jsonMessage, "newMachineID", QString::number(myMachineObject->getMachineID()) ,false);
        endJSONMessage(jsonMessage);

        slaveSocket->write(jsonMessage.toAscii().data());
        slaveSocket->flush();
    }
    else{
        slaveSocket->disconnectFromHost();
    }
}


void ProtoConnect::disconnectMachine(Machine *machine, Management *management){
    //remove the machine from management
    management->removeMachine(machine);
}
