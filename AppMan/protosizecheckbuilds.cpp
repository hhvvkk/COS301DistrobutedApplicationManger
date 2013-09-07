#include "protosizecheckbuilds.h"
#include "management.h"

ProtoSizeCheckBuilds::ProtoSizeCheckBuilds(QObject *parent):
        Protocol(parent)
{
}

void ProtoSizeCheckBuilds::handle(QString data, Management *management, QTcpSocket *slaveSocket){
    if(data.contains("BuildMD5:#")){
        BuildMD5(data, management, slaveSocket);
    }
}

void ProtoSizeCheckBuilds::BuildMD5(QString data, Management *management, QTcpSocket *slaveSocket){
    //this means it is build information that is following.
    //E.g BuildMD5:#1#NameBlah
    QString mostLeft = "BuildMD5:#";

    QString rightSide = data.right((data.size()-mostLeft.length()));
    //E.g. RIGHT SIDE= "1#NameBlah"
    //REASONING behind this method(THE INDEX COUNTING)
    //is that once you have for instance multiple # in name...
    //this prevents problems from occurring
    QString buildNo = rightSide.left(rightSide.indexOf("#"));

    QString buildMD5Value = rightSide.right(rightSide.length() - (buildNo.length()+1));

    QString slaveIp = slaveSocket->peerAddress().toString();

    management->slaveBuildSize(buildNo, buildMD5Value, slaveIp);

}

