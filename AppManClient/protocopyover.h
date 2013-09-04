#ifndef PROTOCOPYOVER_H
#define PROTOCOPYOVER_H

#include "protocol.h"


/**
  * @class ProtoCopyOver
  * @brief This is the protocol for CopyOver
  */
class ProtoCopyOver : public Protocol
{
public:
    ProtoCopyOver(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the CopyOver protocol functions
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void handle(QString data, Management *management, QTcpSocket *masterSocket);

private:
    /**
     * @brief A function that will be invoked when 'CopyBuildOver' has been found in ReadyCheck
     * @param data The data that will be parsed
     */
    void CopyBuildOver(QString data, Management* management, QTcpSocket *masterSocket);
};

#endif // PROTOCOPYOVER_H
