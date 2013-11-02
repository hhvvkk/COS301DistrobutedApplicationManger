#include "protosendbuild.h"
#include "management.h"
#include "protocolhandler.h"
#include "copysenderserver.h"
#include "Build.h"
#include "Slave.h"


ProtoSendBuild::ProtoSendBuild(QObject *parent)
    :Protocol(parent)
{
}

ProtoSendBuild::~ProtoSendBuild(){

    while(sendList.size() > 0){
        //the sendList contents will delete itself(after 30 sec of no connection or when disconnecting)
        sendList.removeFirst();
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
        return;
    }

    int machineId = handler->getMachine()->getMachineID();

    QStringList *differentBuildDirectories = new QStringList();
    QStringList *differentBuildIDs = new QStringList();

    for(int i = 0; i < handler->getMachine()->getBuildCount(); i++){
        Build *aBuild = handler->getMachine()->getBuildAt(i);

        if(aBuild == 0){
            continue;
        }

        if(!aBuild->getIsSame()){//if the build is not the same, append it to the list
            differentBuildIDs->append(QString::number(aBuild->getBuildID()));
            differentBuildDirectories->append(aBuild->getBuildDirectory());
            initiateGetBuildStructure(aBuild->getBuildID(), slaveSocket);
        }
        else{//if they are the same...
            //show that they are the same through the function...(redundency)
            management->setSlaveBuildIsSame(true, machineId, aBuild->getBuildID());
        }
    }


    CopySenderServer *newSender = 0;
    if(newSender == 0){
        newSender = new CopySenderServer(differentBuildDirectories, differentBuildIDs, management, machineId);
        sendList.append(newSender);
        connect(newSender, SIGNAL(copySenderServerDone(CopySenderServer*)), this, SLOT(copySenderServerDone(CopySenderServer*)));
        connect(newSender, SIGNAL(fullySynchronised(int,int,Management*)), this, SLOT(fullySynchronisedBuild(int,int,Management*)));
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

        sendJSONMessage(slaveSocket, jsonMessage);
    }
}

void ProtoSendBuild::sizeCheckCertainBuildDone(/*int buildID, Machine *machine, Management *management,*/ QTcpSocket *slaveSocket){
    /*
     * If it happens that newSender is not null, you dont want to recheck the build information yet.
     * Thus check if it is currently busy checking. if there is a newSender currently checking
     * return...
     *
     */
    if(sendList.size() >= 1){
        return;
        //the reason for returning is that it will recheck all builds after the current are done...
        //thus it is not needed to add another
    }

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "SizeCheckAllBuilds", false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}


void ProtoSendBuild::initiateGetBuildStructure(int buildID, QTcpSocket * slaveSocket){
    //BEFORE anything you create the directory structure on the other side...
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSendStructure", true);
    appendJSONValue(jsonMessage, "subHandler", "InitiateGetStructure", true);
    appendJSONValue(jsonMessage, "buildID", QString::number(buildID), false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}

void ProtoSendBuild::copySenderServerDone(CopySenderServer * deleteCopySender){
    //go and finish the copySender since it is done
    QtConcurrent::run(this, &ProtoSendBuild::removeCopySenderServer, deleteCopySender);
    SizeCheckAllBuilds();
}

void ProtoSendBuild::removeCopySenderServer(CopySenderServer * deleteCopySender){
    sendList.removeOne(deleteCopySender);
    //This means that there is another newSender or it is empty, thus can just delete
    //the NewSender deletes itself...
}

void ProtoSendBuild::SizeCheckAllBuilds(){
    /*The following is to create a loop to continually update
      the build until the information is updated
      **The same is done on startup from a slave machine on protoslavecurrentbuilds.cpp
      */
    QObject *myParent = this->parent();
    if(myParent == 0)
        return;

    ProtocolHandler *handler = dynamic_cast<ProtocolHandler*>(myParent);

    if(handler == 0){
        return;
    }

    handler->recheckAllSizes();
}

void ProtoSendBuild::fullySynchronisedBuild(int intBuildID, int machineId, Management *management){
    management->setSlaveBuildIsSame(true, machineId, intBuildID);
}
