#ifndef PROTOSIZECHECKBUILDS_H
#define PROTOSIZECHECKBUILDS_H

#include "protocol.h"

class Management;

class ProtoSizeCheckBuilds : public Protocol
{
public:
    ProtoSizeCheckBuilds(QObject *parent = 0);

    void handle(QString data, Management *management, QTcpSocket *masterSocket);

    void sendCurrentBuildMD5(Management *management, QTcpSocket *masterSocket);
private:
    int buildIterator;
};

#endif // PROTOSIZECHECKBUILDS_H
