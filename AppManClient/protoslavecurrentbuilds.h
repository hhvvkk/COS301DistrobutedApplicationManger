#ifndef PROTOSLAVECURRENTBUILDS_H
#define PROTOSLAVECURRENTBUILDS_H

#include "protocol.h"
#include "Build.h"


/**
  * @class ProtoSlaveCurrentBuilds
  * @brief This is the protocol for SlaveCurrentBuilds
  */
class ProtoSlaveCurrentBuilds : public Protocol
{
public:
    ProtoSlaveCurrentBuilds(QObject *parent);

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the SlaveCurrentBuild functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);

private:
    /**
     * \fn void Rechecker(Management *management, QTcpSocket *masterSocket);
     * @brief The rechecker part of the protocol in which this machine specifies all the builds it currently has
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void Rechecker(Management *management, QTcpSocket *masterSocket);

private:
    /**
     * @brief A management link that will allow backwards communication
     */
    int buildIterator;
    /**
     * @brief Will be used to loop through the builds
     */
    QList<Build *> allBuilds;
};

#endif // PROTOSLAVECURRENTBUILDS_H
