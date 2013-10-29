#ifndef PROTORUNSIM_H
#define PROTORUNSIM_H

#include "protocol.h"

class ProtoRunSim : public Protocol
{
    Q_OBJECT
public:
    explicit ProtoRunSim(QObject *parent = 0);
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
    void runSim(QTcpSocket *slaveSocket, QString build, QString args);
private:
    void simRunning(QVariantMap jsonObject, Management *management);
};

#endif // PROTORUNSIM_H
