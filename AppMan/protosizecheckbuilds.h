#ifndef PROTOSIZECHECKBUILDS_H
#define PROTOSIZECHECKBUILDS_H

#include "protocol.h"

class ProtoSizeCheckBuilds : public Protocol
{
public:
    ProtoSizeCheckBuilds(QObject *parent = 0);

    void handle(QString data, Management *man, QTcpSocket *slaveSocket);

private:
    void BuildMD5(QString data, Management *management, QTcpSocket *slaveSocket);
};

#endif // PROTOSIZECHECKBUILDS_H
