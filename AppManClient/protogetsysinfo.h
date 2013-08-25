#ifndef PROTOGETSYSINFO_H
#define PROTOGETSYSINFO_H

#include "protocol.h"

class ProtoGetSysInfo : public Protocol
{
public:
    ProtoGetSysInfo(QObject *parent = 0);

    void handle(QString data, Management *management, QTcpSocket *masterSocket);

private:
    void GetAllSysInfo(QString data, Management *management, QTcpSocket *masterSocket);
};

#endif // PROTOGETSYSINFO_H
