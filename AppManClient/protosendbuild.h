#ifndef PROTOSENDBUILD_H
#define PROTOSENDBUILD_H

#include "protocol.h"
#include "copysenderclient.h"

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
     * @param jsonObject A QVariantMap which contains values
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);


private:
    void SendBuildCopyServer(QVariantMap jsonObject, QTcpSocket *masterSocket);
};

#endif // PROTOSENDBUILD_H
