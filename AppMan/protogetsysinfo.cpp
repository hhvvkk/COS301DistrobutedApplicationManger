#include "protogetsysinfo.h"

ProtoGetSysInfo::ProtoGetSysInfo(QObject *parent)
    :Protocol(parent)
{
}

void ProtoGetSysInfo::handle(QString data, Management *management, QTcpSocket *slaveSocket){
    if(data.contains("DetailedSysInfoFollows:#"))
        detSysInfoFollows(data, management);
    if(data.contains("MinimalSysInfoFollows:#"))
        minSysInfoFollows(data, management);
}

void ProtoGetSysInfo::detSysInfoFollows(QString data, Management * management){
    QString stripped = data;
    stripped = stripped.right(data.length()-24);
    qDebug()<<'\n'<<'\n';
    qDebug()<<stripped;
    //emit setDetStats(stripped);
}

void ProtoGetSysInfo::minSysInfoFollows(QString data, Management *management){

    QString stripped = data;
    stripped = stripped.right(data.length()-22);
    qDebug()<<'\n'<<'\n';
    qDebug()<<stripped;
    //emit setMinStats(stripped);
}

void ProtoGetSysInfo::getDetailed(QTcpSocket *slaveSocket){
    slaveSocket->write("||GetDetailedSysInfo||");
    slaveSocket->flush();
}

void ProtoGetSysInfo::getMinimal(QTcpSocket *slaveSocket){
    slaveSocket->write("||GetMinimalSysInfo||");
    slaveSocket->flush();
}
