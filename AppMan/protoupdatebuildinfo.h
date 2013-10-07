#ifndef PROTOUPDATE_H
#define PROTOUPDATE_H

#include "protocol.h"




/**
  * @class ProtoUpdateBuildInfo
  * @brief This class is the protocol class handling the update of build information to the slave machine
  */
class ProtoUpdateBuildInfo : public Protocol
{
    Q_OBJECT
public:

    /**
     * \fn ProtoUpdateBuildInfo(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoUpdateBuildInfo(QObject *parent = 0);


    /**
     * \fn ProtoUpdateBuildInfo();
     * @brief the destructor
     */
    ~ProtoUpdateBuildInfo();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the Copy over functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);



    /**
     * \fn void updateName(int buildID, QString newBuildName);
     * @brief A function which writes to the slave machine the new build updated Name
     * @param buildID the management to perform functions on
     * @param newBuildName the socket if it is needed to write to it
     * @param slaveSocket the slave machine socket to write the new updated information
     */
    void updateName(int buildID, QString newBuildName, QTcpSocket *slaveSocket);


    /**
     * \fn void UpdatedBuildName(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket);
     * @brief A function which fires in the event that the build name is updated sucessfully on the slave machine
     * @param jsonObject A QVariantMap which contains values
     * @param management the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void UpdatedBuildName(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket);
};

#endif // PROTOUPDATE_H
