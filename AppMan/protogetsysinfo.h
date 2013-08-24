#ifndef PROTOGETSYSINFO_H
#define PROTOGETSYSINFO_H

#include "protocol.h"

class ProtoGetSysInfo : public Protocol
{
public:
    ProtoGetSysInfo(QObject *parent = 0);

    void handle(QString data, Management *man, QTcpSocket *slaveSocket);

private:
    void SysInfoFollows(QString data, Management *management);
};

#endif // PROTOGETSYSINFO_H
