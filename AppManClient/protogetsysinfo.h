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
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the GetSysInfo protocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);

private:
    void GetDetSysInfo(Management *management, QTcpSocket *masterSocket);
    void GetMinSysInfo(Management *management, QTcpSocket *masterSocket);
};

#endif // PROTOGETSYSINFO_H
