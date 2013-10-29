#ifndef PROTORUNSIM_H
#define PROTORUNSIM_H

#include "protocol.h"

class ProtoRunSim : public Protocol
{
public:
    ProtoRunSim(QObject *parent = 0);
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);
    void runSim(Management *management, QTcpSocket *masterSocket, QString build, QString arg);
   // void simRunning(Management *management, QTcpSocket *masterSocket);
};

#endif // PROTORUNSIM_H
