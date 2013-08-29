#include "protosendbuild.h"
#include "copysenderserver.h"
#include "management.h"
#include "protocolhandler.h"
#include "Build.h"


ProtoSendBuild::ProtoSendBuild(QObject *parent)
    :Protocol(parent)
{
}

void ProtoSendBuild::handle(QString data, Management *management, QTcpSocket *slaveSocket){
    if(!data.compare("SizeCheckAllBuildsDone"))
        SizeCheckAllBuildsDone(slaveSocket);
}

void ProtoSendBuild::SizeCheckAllBuildsDone(QTcpSocket *slaveSocket){

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

    for(int i = 0; i < handler->getMachine()->getBuildCount(); i++){
        Build aBuild = slaveBuilds[i];
        if(!aBuild.getIsSame()){//if the build is not the same, append it to the list
            differentBuildDirectories.append(aBuild.getBuildDirectory());
        }
    }

    qDebug()<<"QStringList size = "<<differentBuildDirectories.size();

    for(int i = 0; i < differentBuildDirectories.size(); i++){
        qDebug()<<differentBuildDirectories.at(i);
    }
    //here on  out this will create a class that will synchronize the folders
    CopySenderServer * newSender = new CopySenderServer(differentBuildDirectories);
   // newSender->startServer(slaveSocket);

}
