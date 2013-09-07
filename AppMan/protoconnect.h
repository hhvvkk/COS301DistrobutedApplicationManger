#ifndef PROTOCONNECT_H
#define PROTOCONNECT_H

#include "protocol.h"

class Machine;

class ProtoConnect : public Protocol
{
public:
    /**
     * \fn ProtoConnect(QObject *parent = 0);
     * @brief The constructor
     */
    explicit ProtoConnect(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the ConnectProtocol functions
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QString data, Management *man, QTcpSocket *slaveSocket);

    /**
     * \fn void disconnectMachine(Machine *machine, Management *management);
     * @brief This function will be invoked once a slave disconnects
     * @param machine The machine that will disconnect
     * @param management A pointer to management to be able to delete the machine
     */
    void disconnectMachine(Machine *machine, Management *management);

};

#endif // PROTOCONNECT_H
