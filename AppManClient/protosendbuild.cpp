#include "protosendbuild.h"

ProtoSendBuild::ProtoSendBuild(QObject *parent)
    :Protocol(parent)
{
}

void ProtoSendBuild::handle(QString data, Management *management, QTcpSocket *masterSocket){    
    if(data.contains("SendBuildCopyServer:#"))
        SendBuildCopyServer(data, masterSocket);
}

void ProtoSendBuild::SendBuildCopyServer(QString data, QTcpSocket *masterSocket){
    qDebug()<<"PROTOSENDHANDLE--PROTOCLAss";

    //E.g SendBuildCopyServer:#121.110.222.212#4412
    QString mostLeft = "SendBuildCopyServer:#";

    QString hostPort = data.right((data.size()-mostLeft.length()));
    //E.g. RIGHT SIDE= "4412"

    QHostAddress theHostAddress = masterSocket->peerAddress();
    if(theHostAddress.isNull())
        return;
    //QString hostPort = rightSide.left(rightSide.indexOf("#"));


    qDebug()<<"HOSTAd"<<theHostAddress.toString();
    qDebug()<<"HOSTPor"<<hostPort;

    //check to see if port is actually valid
    bool ok;
    int port = hostPort.toInt(&ok);

    if(!ok)
        return;

    CopySenderClient *senderClient = new CopySenderClient(theHostAddress, port);

    bool connected = senderClient->connectToHost();

    if(!connected){
        qDebug()<<"NOT CONNECTED";
        senderClient->deleteLater();
    }
    //else if it is successfully connected and should continue on its own
    senderClient = 0;
}
