#include "protogetsysinfo.h"
#include "management.h"

ProtoGetSysInfo::ProtoGetSysInfo(QObject *parent)
    :Protocol(parent)
{
}

void ProtoGetSysInfo::handle(QString data, Management *management, QTcpSocket *masterSocket) {
    if(!data.compare("GetAllSysInfo"))
        GetAllSysInfo(data, management, masterSocket);
}

void ProtoGetSysInfo::GetAllSysInfo(QString data, Management *management, QTcpSocket *masterSocket){
    data.compare("Find the splitters");
    management->parent();//possibly use management to get system info,
                        //otherwise initiate a sysinfo class inside this class constructor
                        //write the string SysInfoFollows:#[sysInfo--delimeted as you wish]
    masterSocket->write("SysInfoFollows:#");
    masterSocket->flush();
}
