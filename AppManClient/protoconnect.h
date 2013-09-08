#ifndef PROTOCONNECT_H
#define PROTOCONNECT_H

#include "protocol.h"
#include <QSettings>

//forward declaration for use
class ProtocolHandler;


/**
  * @class ProtoConnect
  * @brief This is the protocol for Connect
  */
class ProtoConnect : public Protocol
{
public:
    /**
     * \fn ProtoConnect(QObject *parent = 0);
     * @brief The constructor
     */
    ProtoConnect(QObject *parent = 0);

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the Connect protocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);

    /**
     * \fn void disconnectFromMaster(Management *management, QTcpSocket *socket);
     * @brief This function will be invoked once this machine disconnects
     * @param management The management pointer to perform functions on
     * @param socket The socket to disconnect
     */
    void disconnectFromMaster(Management *management, QTcpSocket *socket);

    /**
     * \fn void connectToMaster(QString ipAddress, int serverPort, ProtocolHandler * protocolHandler);
     * @brief This function will be used to connect to the master and will set the protocolHandler's socket as well as new SocketClient socket
     * @param ipAddress The ip address of the machine to connect to
     * @param serverPort The port on which the server to connect to will be
     * @param protocolHandler The protocol handler to connect to a certain socket
     */
    void connectToMaster(QString ipAddress, int serverPort, ProtocolHandler * protocolHandler);


private:
    /**
     * \fn void initiateSlaveCurrentBuilds(QTcpSocket *masterSocket);
     * @brief This function will be initiated once it is successfully connected to the master to invoke SlaveCurrentBuilds protocol
     * @param masterSocket The socket to which this function can write to to initiate SlaveCurrentBuilds
     */
    void initiateSlaveCurrentBuilds(QTcpSocket *masterSocket);

    /**
     * \fn void initiateSlaveCurrentBuilds(QTcpSocket *masterSocket);
     * @brief This function will be initiated once it is successfully connected to the master to invoke SlaveCurrentBuilds protocol
     * @return Returns this machines ID or returns -1 if invalid or non existant, further will replace current id if it is invalid
     */
    QString getThisIDBySettings();

    /**
     * \fn void SetMachineID(int newMachineID);
     * @brief This function will save the machineID value into the settings file for this machine
     * @param newMachineID The new Machine ID for this machine
     */
    void SetMachineID(int newMachineID);

private:
    /**
      * @var machineID
      * @brief The machine Id stored inside the ini file
      */
    int machineID;
};

#endif // PROTOCONNECT_H
