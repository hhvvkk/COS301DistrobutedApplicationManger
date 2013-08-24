#ifndef PROTOCONNECT_H
#define PROTOCONNECT_H

#include "protocol.h"

class ProtoConnect : public Protocol
{
public:
    ProtoConnect(QObject *parent = 0);

    void handle(QString data, Management *management, QTcpSocket *masterSocket);

    void disconnectedFromMaster(Management *management);

private:
    void initiateSlaveCurrentBuilds(QTcpSocket *masterSocket);
};

#endif // PROTOCONNECT_H
