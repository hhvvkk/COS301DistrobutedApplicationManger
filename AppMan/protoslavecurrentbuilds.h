#ifndef PROTOSLAVECURRENTBUILDS_H
#define PROTOSLAVECURRENTBUILDS_H

#include "protocol.h"

class Management;

class ProtoSlaveCurrentBuilds : public Protocol
{
public:
    /**
     * \fn ProtoSlaveCurrentBuilds(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoSlaveCurrentBuilds(QObject *parent = 0);

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
      * \fn void RecheckDone(QTcpSocket *slaveSocket);
      * @brief This function will be invoked if RecheckDone has been found in the data
      * @param slaveSocket The socket on which the function will reply to
      */
    void RecheckDone(QTcpSocket *slaveSocket);


    /**
     * \fn void RecheckBuilds(QTcpSocket *slaveSocket);
     * @brief A function invoked when the ReadyRead observes 'RecheckBuilds' in the data string and parses it
     * @param data The data to be parsed by this function
     */
    void RecheckBuilds(QTcpSocket *slaveSocket);


    /**
     * \fn void Rechecker(QString data, Management *management, QTcpSocket *socket);
     * @brief A function invoked when the ReadyRead observes 'Rechecker' in the data string and parses it
     * @param data The data to be parsed by this function
     */
    void Rechecker(QString data, Management *management, QTcpSocket *socket);


    /**
      * \fn void SizeCheckAllBuilds(Management *management);
      * @brief A function that will be invoked to initiate a size check for all builds
      * @param slaveSocket The socket to which the request will be sent to
      */
    void SizeCheckAllBuilds(QTcpSocket *slaveSocket);

};

#endif // PROTOSLAVECURRENTBUILDS_H
