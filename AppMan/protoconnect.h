#ifndef PROTOCONNECT_H
#define PROTOCONNECT_H

#include "protocol.h"

class Machine;

/**
  * @class ProtoConnect
  * @brief This class is the protocol class handling protocols for connect and disconnect of machines with the master machine
  */
class ProtoConnect : public Protocol
{
    Q_OBJECT
public:
    /**
     * \fn ProtoConnect(QObject *parent = 0);
     * @brief The constructor
     */
    explicit ProtoConnect(QObject *parent = 0);


    /**
     * \fn ~ProtoConnect();
     * @brief The destructor for the protocol
     */
    ~ProtoConnect();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the ConnectProtocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);

    /**
     * \fn void disconnectMachine(Machine *machine, Management *management);
     * @brief This function will be invoked once a slave disconnects
     * @param machine The machine that will disconnect
     * @param management A pointer to management to be able to delete the machine
     */
    void disconnectMachine(Machine *machine, Management *management);

};

#endif // PROTOCONNECT_H
