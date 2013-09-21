#include "protosendbuild.h"
#include "management.h"
#include "protocolhandler.h"
#include "copysenderserver.h"
#include "Build.h"
#include "Slave.h"


ProtoSendBuild::ProtoSendBuild(QObject *parent)
    :Protocol(parent)
{
    //lock = new QMutex();
}

ProtoSendBuild::~ProtoSendBuild(){
//    if(lock != 0){
//        delete lock;
//    }
    for(int i = 0; i < sendList.size(); i++){
        CopySenderServer *copySender = sendList.at(i);
        if(copySender != 0){
             sendList.removeOne(copySender);
            copySender->deleteLater();
        }
    }
}

void ProtoSendBuild::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("SizeCheckAllBuildsDone"))
        SizeCheckAllBuildsDone(slaveSocket, management);
}

void ProtoSendBuild::SizeCheckAllBuildsDone(QTcpSocket *slaveSocket, Management *management){
    if(slaveSocket == 0){
        return;
    }

    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    if(handler == 0){
        qDebug()<<"handler=0";
        return;
    }

    Build *slaveBuilds = handler->getMachine()->getBuilds();

    QStringList *differentBuildDirectories = new QStringList();
    QStringList *differentBuildNos = new QStringList();;

    for(int i = 0; i < handler->getMachine()->getBuildCount(); i++){
        Build aBuild = slaveBuilds[i];
        if(!aBuild.getIsSame()){//if the build is not the same, append it to the list
            differentBuildNos->append(QString::number(aBuild.getBuildID()));
            differentBuildDirectories->append(aBuild.getBuildDirectory());
        }
    }

    int machineId = handler->getMachine()->getMachineID();

    CopySenderServer *newSender = 0;
    if(newSender == 0){
        newSender = new CopySenderServer(differentBuildDirectories, differentBuildNos, management, machineId);
        sendList.append(newSender);
        connect(newSender, SIGNAL(copySenderServerDone(CopySenderServer*)), this, SLOT(copySenderServerDone(CopySenderServer*)));
    }

    //get the port on which new server will run
    int port = newSender->startServer();


    if(port == 0){
        newSender->deleteLater();
        return;
    }
    else{

        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "ProtoSendBuild", true);
        appendJSONValue(jsonMessage, "subHandler", "SendBuildCopyServer", true);
        appendJSONValue(jsonMessage, "hostPort", QString::number(port), false);
        endJSONMessage(jsonMessage);

        slaveSocket->write(jsonMessage.toAscii().data());
        slaveSocket->flush();
    }
}

void ProtoSendBuild::sizeCheckCertainBuildDone(/*int buildID, Machine *machine, Management *management,*/ QTcpSocket *slaveSocket){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "SizeCheckAllBuilds", false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toAscii().data());
    slaveSocket->flush();

//    QStringList *differentBuildDirectories = new QStringList();
//    QStringList *differentBuildNos = new QStringList();;

//    Build *slaveBuilds = machine->getBuilds();

//    for(int i = 0; i < machine->getBuildCount(); i++){
//        Build aBuild = slaveBuilds[i];
//        if(buildID == aBuild.getBuildID()){
//            differentBuildNos->append(QString::number(aBuild.getBuildID()));
//            differentBuildDirectories->append(aBuild.getBuildDirectory());
//            break;
//        }
//    }

//    if(slaveSocket == 0){
//        return;
//    }

//    int machineId = machine->getMachineID();


//    CopySenderServer *newSender = 0;
//    if(newSender == 0){
//        newSender = new CopySenderServer(differentBuildDirectories, differentBuildNos, management, machineId);
//        sendList.append(newSender);
//        qDebug()<<newSender->isListening();
//        connect(newSender, SIGNAL(copySenderServerDone(CopySenderServer*)), thisPointer, SLOT(copySenderServerDone(CopySenderServer*)));
//    }
//    //get the port on which new server will run
//    int port = newSender->startServer();

//    if(port == 0){
//        newSender->deleteLater();
//        return;
//    }
//    else{
//        QString jsonMessage = startJSONMessage();
//        appendJSONValue(jsonMessage, "handler", "ProtoSendBuild", true);
//        appendJSONValue(jsonMessage, "subHandler", "SendBuildCopyServer", true);
//        appendJSONValue(jsonMessage, "hostPort", QString::number(port), false);
//        endJSONMessage(jsonMessage);

//        slaveSocket->write(jsonMessage.toAscii().data());
//        slaveSocket->flush();
//    }
}


void ProtoSendBuild::copySenderServerDone(CopySenderServer * deleteCopySender){
    //go and finish the copySender since it is done

    sendList.removeOne(deleteCopySender);

    //This means that there is another newSender or it is empty, thus can just delete
    while(deleteCopySender->isBusyDeleting())
    {/*Wait untill it is finished deleting*/}

    deleteCopySender->deleteLater();

}
