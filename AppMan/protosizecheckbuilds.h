#ifndef PROTOSIZECHECKBUILDS_H
#define PROTOSIZECHECKBUILDS_H

#include "protocol.h"

class ProtoSizeCheckBuilds : public Protocol
{
public:
    /**
     * \fn ProtoSlaveCurrentBuilds(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoSizeCheckBuilds(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the ConnectProtocol functions
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QString data, Management *man, QTcpSocket *slaveSocket);

private:

    /**
     * \fn void BuildMD5(QString data, Management *management, QTcpSocket *slaveSocket);
     * @brief The function to invoke once "BuildMD5" has been observed
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void BuildMD5(QString data, Management *management, QTcpSocket *slaveSocket);
};

#endif // PROTOSIZECHECKBUILDS_H
