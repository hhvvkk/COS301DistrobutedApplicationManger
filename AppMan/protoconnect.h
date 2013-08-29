#ifndef PROTOCONNECT_H
#define PROTOCONNECT_H

#include "protocol.h"

class Machine;

class ProtoConnect : public Protocol
{
public:
    ProtoConnect(QObject *parent = 0);

    void handle(QString data, Management *man, QTcpSocket *slaveSocket);

    void disconnectMachine(Machine *machine, Management *management);

};

#endif // PROTOCONNECT_H
