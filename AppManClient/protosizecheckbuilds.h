#ifndef PROTOSIZECHECKBUILDS_H
#define PROTOSIZECHECKBUILDS_H

#include "protocol.h"

class Management;


/**
  * @class ProtoSizeCheckBuilds
  * @brief This is the protocol for SizeCheckBuilds
  */
class ProtoSizeCheckBuilds : public Protocol
{
public:
    ProtoSizeCheckBuilds(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the SizeCheckBuild protocol functions
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void handle(QString data, Management *management, QTcpSocket *masterSocket);

    /**
     * \fn void sendCurrentBuildMD5(Management *management, QTcpSocket *masterSocket);
     * @brief A function looping through the builds and sending all the builds that this machine has
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void sendCurrentBuildMD5(Management *management, QTcpSocket *masterSocket);

    /**
     * \fn void sendABuildMD5(QString data, Management *management, QTcpSocket *masterSocket);
     * @brief A function similar to sendCurrentBuildMD5 but with a difference - it only sends the buildMD5Sum of the specified build
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void sendABuildMD5(QString data, Management *management, QTcpSocket *masterSocket);

private:
    /**
      * @var buildIterator
      * @brief This is a build iterator to loop through all the builds to send md5 values to master machine
      */
    int buildIterator;
};

#endif // PROTOSIZECHECKBUILDS_H
