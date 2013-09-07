#ifndef PROTOGETSYSINFO_H
#define PROTOGETSYSINFO_H

#include "protocol.h"

class ProtoGetSysInfo : public Protocol
{
public:
    /**
     * \fn ProtoSlaveCurrentBuilds(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoGetSysInfo(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the ConnectProtocol functions
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QString data, Management *man, QTcpSocket *slaveSocket);

    void getMinimal(QTcpSocket *slaveSocket);
    void getDetailed(QTcpSocket *slaveSocket);

private:
    void detSysInfoFollows(QString data, Management *management);
    void minSysInfoFollows(QString data, Management *management);
signals:
    //void setDetStats(QString stats);
    //void setMinStats(QString stats);
};

#endif // PROTOGETSYSINFO_H
