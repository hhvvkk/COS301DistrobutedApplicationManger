#include "protosendbuild.h"

ProtoSendBuild::ProtoSendBuild(QObject *parent)
    :Protocol(parent)
{
}

void ProtoSendBuild::handle(QString data, Management *management, QTcpSocket *masterSocket){
    if(data.contains("SendBuildCopyServer:#"))
        SendBuildCopyServer(data, management, masterSocket);
}

void ProtoSendBuild::SendBuildCopyServer(QString data, Management *management, QTcpSocket *masterSocket){
    qDebug()<<"PROTOSENDHANDLE--PROTOCLAss";

    //E.g SendBuildCopyServer:#121.110.222.212#4412
    QString mostLeft = "SendBuildCopyServer:#";

    QString rightSide = data.right((data.size()-mostLeft.length()));
    //E.g. RIGHT SIDE= "121.110.222.212#4412"

    QString hostAddress = rightSide.left(rightSide.indexOf("#"));
    QString hostPort = rightSide.right(rightSide.length() - rightSide.indexOf("#")-1);

    qDebug()<<"HOSTAd"<<hostAddress;
    qDebug()<<"HOSTPor"<<hostPort;

    //check to see if port is actually valid
    bool ok;
    int port = hostPort.toInt(&ok);

    if(!ok)
        return;

    CopySenderClient *senderClient = new CopySenderClient(hostAddress, port);

    bool connected = senderClient->connectToHost();

    if(!connected){
        qDebug()<<"NOT CONNECTED";
        senderClient->deleteLater();
    }
    //else if it is successfully connected and should continue on its own
    senderClient = 0;
}
