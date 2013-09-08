#ifndef PROTOSENDBUILD_H
#define PROTOSENDBUILD_H

#include "protocol.h"

/**
  * @class ProtoSendBuild
  * @brief This is the protocol for SendBuild
  */
class ProtoSendBuild : public Protocol
{
public:
    /**
     * \fn ProtoSendBuild(QObject *parent = 0);
     * @brief The constructor
     */
    ProtoSendBuild(QObject *parent = 0);

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the SendBuild protocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);


    /**
     * \fn void SizeCheckAllBuildsDone();
     * @brief This function will be invoked once SizeCheckAllBuilds has completed and will resynchronize the builds
     * @param slaveSocket The socket if it is needed to write to it
     */
    void SizeCheckAllBuildsDone(QTcpSocket *slaveSocket,  Management *management);

    void sizeCheckCertainBuildDone();
};

#endif // PROTOSENDBUILD_H
