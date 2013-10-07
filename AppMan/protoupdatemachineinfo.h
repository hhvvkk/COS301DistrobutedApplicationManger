#ifndef PROTOUPDATEMACHINEINFO_H
#define PROTOUPDATEMACHINEINFO_H

#include "protocol.h"



/**
  * @class ProtoUpdateBuildInfo
  * @brief This class is the protocol class handling the update of machine information
  */

class ProtoUpdateMachineInfo : public Protocol
{
    Q_OBJECT
public:

    /**
     * \fn ProtoUpdateMachineInfo(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoUpdateMachineInfo(QObject *parent = 0);


    /**
     * \fn ProtoUpdateMachineInfo();
     * @brief the destructor
     */
    ~ProtoUpdateMachineInfo();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the Copy over functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);

    /**
    * \fn void updateUniqueID(int uniqueID);
    * @brief The function to update the unique ID on the machine
    * @param uniqueID the new ID of the machine
    * @param slaveSocket The socket to write to
    */
    void updateUniqueID(int uniqueID, QTcpSocket *slaveSocket);
};

#endif // PROTOUPDATEMACHINEINFO_H
