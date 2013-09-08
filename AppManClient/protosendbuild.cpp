#include "protosendbuild.h"

ProtoSendBuild::ProtoSendBuild(QObject *parent)
    :Protocol(parent)
{
}

void ProtoSendBuild::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){
    if(!jsonObject.value("subHandler").toString().compare("SendBuildCopyServer"))
        SendBuildCopyServer(jsonObject, masterSocket);
}

void ProtoSendBuild::SendBuildCopyServer(QVariantMap jsonObject, QTcpSocket *masterSocket){

    QString hostPort = jsonObject.value("hostPort").toString();

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
