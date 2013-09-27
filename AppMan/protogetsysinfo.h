#ifndef PROTOGETSYSINFO_H
#define PROTOGETSYSINFO_H

#include "protocol.h"

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
     * @brief The function that will handle all the ConnectProtocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);

    void getMinimal(QTcpSocket *slaveSocket);
    void getDetailed(QTcpSocket *slaveSocket);

private:
    void MinimalSysInfoFollows(QVariantMap jsonObject, Management *management);
    void DetailedSysInfoFollows(QVariantMap jsonObject, Management *management);
signals:
    //void setDetStats(QString stats);
    //void setMinStats(QString stats);
};

#endif // PROTOGETSYSINFO_H
