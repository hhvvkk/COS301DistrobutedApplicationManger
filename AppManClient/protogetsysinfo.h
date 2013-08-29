#ifndef PROTOGETSYSINFO_H
#define PROTOGETSYSINFO_H

#include "protocol.h"


/**
  * @class ProtoGetSysInfo
  * @brief This is the protocol for GetSysInfo
  */
class ProtoGetSysInfo : public Protocol
{
public:
    ProtoGetSysInfo(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the GetSysInfo protocol functions
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QString data, Management *management, QTcpSocket *masterSocket);

private:
    void GetAllSysInfo(QString data, Management *management, QTcpSocket *masterSocket);
};

#endif // PROTOGETSYSINFO_H
