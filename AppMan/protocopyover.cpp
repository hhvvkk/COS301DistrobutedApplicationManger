#include "protocopyover.h"
#include "management.h"
#include <QHostAddress>

ProtoCopyOver::ProtoCopyOver(QObject *parent) :
    Protocol(parent)
{
}

void ProtoCopyOver::handle(QString data, Management *management, QTcpSocket *slaveSocket){
    if(data.contains("GotABuild:#"))
        GotABuild(data, management, slaveSocket);
}

void ProtoCopyOver::GotABuild(QString data, Management *management, QTcpSocket *slaveSocket){
    //at this point of time in communication, a build has just been added
    //because 1. it was already there or 2. it has just been added
    //"GotABuild:#"+buildNo;
    QHostAddress adr = slaveSocket->peerAddress();

    QString leftSide = data.left(data.indexOf("#"));
    QString buildNo = data.right(data.length() - (leftSide.length()+1));
    QString slaveIp = adr.toString();
    management->addBuildToSlave(slaveIp, buildNo);

    //finally call size check on that build
    sizeCheckCertainBuild(buildNo, slaveSocket);
}


void ProtoCopyOver::copyBuildOver(int buildId, QString buildName, QTcpSocket *slaveSocket){
    QString buildIdString = QString::number(buildId);
    QString writeBuildMessage = "||CopyBuildOver:#"+ buildIdString +"#"+buildName+"||";
    slaveSocket->write(writeBuildMessage.toAscii().data());
    slaveSocket->flush();//write all that should be written
}

void ProtoCopyOver::sizeCheckCertainBuild(QString buildNo, QTcpSocket *slaveSocket){
    QString sizeCheckMessage = "||SizeCheckABuild:#" + buildNo +"||";
    slaveSocket->write(sizeCheckMessage.toAscii().data());
    slaveSocket->flush();
}
