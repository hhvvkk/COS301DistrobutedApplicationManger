#ifndef PROTOCONNECT_H
#define PROTOCONNECT_H

#include "protocol.h"

//forward declaration for use
class ProtocolHandler;

class ProtoConnect : public Protocol
{
public:
    ProtoConnect(QObject *parent = 0);

    void handle(QString data, Management *management, QTcpSocket *masterSocket);

    void disconnectFromMaster(Management *management, QTcpSocket *socket);

    void connectToMaster(QString ipAddress, int serverPort, ProtocolHandler * protocolHandler);
private:
    void initiateSlaveCurrentBuilds(QTcpSocket *masterSocket);
};

#endif // PROTOCONNECT_H
