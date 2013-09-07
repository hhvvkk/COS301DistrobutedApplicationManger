#include "protosendbuild.h"
#include "copysenderserver.h"
#include "management.h"
#include "protocolhandler.h"
#include "Build.h"


ProtoSendBuild::ProtoSendBuild(QObject *parent)
    :Protocol(parent)
{
    ipAddress = "";
}

void ProtoSendBuild::handle(QString data, Management *management, QTcpSocket *slaveSocket){
    if(!data.compare("SizeCheckAllBuildsDone"))
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

    QStringList differentBuildDirectories;
    QStringList differentBuildNos;

    for(int i = 0; i < handler->getMachine()->getBuildCount(); i++){
        Build aBuild = slaveBuilds[i];
        if(!aBuild.getIsSame()){//if the build is not the same, append it to the list
            differentBuildNos.append(QString::number(aBuild.getBuildID()));
            differentBuildDirectories.append(aBuild.getBuildDirectory());
        }
    }


    //here on  out this will create a class that will synchronize the folders
    if(!ipAddress.compare("")){
        qDebug()<<"IP ADDRESS is empty(ProtoSendBuild .cpp -- line 52+";
    }

    CopySenderServer * newSender = new CopySenderServer(differentBuildDirectories, differentBuildNos, management);

    //get the port on which new server will run
    int port = newSender->startServer();

    if(port == 0){
        newSender->deleteLater();
        return;
    }

    QString message = "||SendBuildCopyServer:#"+QString::number(port)+"||";

    slaveSocket->write(message.toAscii().data());
    slaveSocket->flush();
}

void ProtoSendBuild::sizeCheckCertainBuildDone(){
    //////if you copy over a build
}

void ProtoSendBuild::setSendBuildIp(QString ip){
    ipAddress = ip;
}
