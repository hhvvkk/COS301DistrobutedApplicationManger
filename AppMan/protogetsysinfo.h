#ifndef PROTOGETSYSINFO_H
#define PROTOGETSYSINFO_H

#include "protocol.h"

/**
  * @class ProtoGetSysInfo
  * @brief This class is the protocol class handling protocols for getting system infromation from slave machines
  */
class ProtoGetSysInfo : public Protocol
{
    Q_OBJECT
public:
    /**
     * \fn ProtoSlaveCurrentBuilds(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoGetSysInfo(QObject *parent = 0);

    /**
     * \fn ~ProtoGetSysInfo();
     * @brief The destructor for the protocol
     */
    ~ProtoGetSysInfo();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the get system info functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief A function to invoke the getMinimal system information from the slave machine
     * @param slaveSocket The socket of the slave machine where the minimal system information is obtained from
     */
    void getMinimal(QTcpSocket *slaveSocket);

    /**
     * \fn void getDetailed(QTcpSocket *slaveSocket);
     * @brief A function to invoke the getDetailed system information from the slave machine
     * @param slaveSocket The socket of the slave machine where the detailed system information is obtained from
     */
    void getDetailed(QTcpSocket *slaveSocket);

private:
    /**
     * \fn void getDetailed(QTcpSocket *slaveSocket);
     * @brief The function executed from handle when MinimalSysInfo subhandler is called which the minimal system info is stored in
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     */
    void MinimalSysInfoFollows(QVariantMap jsonObject, Management *management);

    /**
     * \fn void getDetailed(QTcpSocket *slaveSocket);
     * @brief The function executed from handle when MinimalSysInfo subhandler is called which the detailed system info is stored in
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     */
    void DetailedSysInfoFollows(QVariantMap jsonObject, Management *management);
signals:
    //void setDetStats(QString stats);
    //void setMinStats(QString stats);
};

#endif // PROTOGETSYSINFO_H
