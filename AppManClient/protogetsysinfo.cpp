#include "protogetsysinfo.h"
#include "management.h"

ProtoGetSysInfo::ProtoGetSysInfo(QObject *parent)
    :Protocol(parent)
{
}

void ProtoGetSysInfo::handle(QString data, Management *management, QTcpSocket *masterSocket) {
    if(!data.compare("GetDetailedSysInfo"))
        GetDetSysInfo(data, management, masterSocket);
    if(!data.compare("GetMinimalSysInfo"))
        GetMinSysInfo(data, management, masterSocket);
}

void ProtoGetSysInfo::GetDetSysInfo(QString data, Management *management, QTcpSocket *masterSocket){
    QString detinf = "||DetailedSysInfoFollows:#"+management->getDetSysInfo()+"||";
    masterSocket->write(detinf.toAscii().data());
    masterSocket->flush();
}

void ProtoGetSysInfo::GetMinSysInfo(QString data, Management *management, QTcpSocket *masterSocket){
    QString detinf = "||MinimalSysInfoFollows:#"+management->getMinSysInfo()+"||";
    masterSocket->write(detinf.toAscii().data());
    masterSocket->flush();
}
