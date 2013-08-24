#include "protoslavecurrentbuilds.h"
#include "management.h"

ProtoSlaveCurrentBuilds::ProtoSlaveCurrentBuilds(QObject *parent)
    :Protocol(parent)
{
    buildIterator = 0;
    allBuilds = 0;
}

void ProtoSlaveCurrentBuilds::handle(QString data, Management *management, QTcpSocket *masterSocket){
    if(!data.compare("RecheckCopy"))
        Rechecker(management, masterSocket);
}

void ProtoSlaveCurrentBuilds::Rechecker(Management *management, QTcpSocket *masterSocket){
    QString rechecker = "";

    if(allBuilds == 0)
        allBuilds = management->getAllBuilds();

    if(buildIterator < management->getBuildCount()){
        rechecker = "||Rechecker:#"+QString::number(allBuilds[buildIterator].getBuildID())+"#"+allBuilds[buildIterator].getBuildName()+"||";
        masterSocket->write(rechecker.toAscii().data());
        masterSocket->flush();
        buildIterator++;
    }
    else{
        //reset the iterator
        allBuilds = 0;
        buildIterator = 0;
        masterSocket->write("||RecheckDone||");
        masterSocket->flush();
    }
}
