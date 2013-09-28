#ifndef PROTOSIZECHECKBUILDS_H
#define PROTOSIZECHECKBUILDS_H

#include "protocol.h"

class ProtoSizeCheckBuilds : public Protocol
{
    Q_OBJECT
public:
    /**
     * \fn ProtoSlaveCurrentBuilds(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoSizeCheckBuilds(QObject *parent = 0);

    /**
     * \fn ~ProtoSizeCheckBuilds();
     * @brief The destructor for the protocol
     */
    ~ProtoSizeCheckBuilds();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the ConnectProtocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);


    /**
     * \fn void invokeSizeCheckAll(QTcpSocket *slaveSocket);
     * @brief The function is there to reinvoke the size check for all builds so that it can enter a loop and ensure the build is fully synched
     * @param slaveSocket the socket if it is needed to write to it
     */
    void invokeSizeCheckAll(QTcpSocket *slaveSocket);

private:

    /**
     * \fn void BuildMD5(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket);
     * @brief The function to invoke once "BuildMD5" has been observed
     * @param jsonObject A QVariantMap object containing build information to be parsed
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void BuildMD5(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket);
};

#endif // PROTOSIZECHECKBUILDS_H
