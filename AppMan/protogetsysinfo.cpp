#include "protogetsysinfo.h"

ProtoGetSysInfo::ProtoGetSysInfo(QObject *parent)
    :Protocol(parent)
{
}

void ProtoGetSysInfo::handle(QString data, Management *management, QTcpSocket *slaveSocket){
    if(data.contains("SysInfoFollows:#"))
        SysInfoFollows(data, management);
}

void ProtoGetSysInfo::SysInfoFollows(QString data, Management *management){
    //do something with the system info.......
}
