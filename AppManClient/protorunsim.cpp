#include "protorunsim.h"
#include "management.h"

ProtoRunSim::ProtoRunSim(QObject *parent):Protocol(parent)
{
}

void ProtoRunSim::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){
    qDebug()<<"void ProtoRunSim::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){";
    if(!jsonObject.value("subHandler").toString().compare("runSim")){
        runSim(management, masterSocket, jsonObject.value("buildID").toString() ,jsonObject.value("args").toString());

    }
}

void ProtoRunSim::runSim(Management * man, QTcpSocket *mas, QString build, QString arg){
    qDebug()<<build<<arg;
    man->runThisSim(build,arg);
}
