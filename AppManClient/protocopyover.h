#ifndef PROTOCOPYOVER_H
#define PROTOCOPYOVER_H

#include "protocol.h"

class ProtoCopyOver : public Protocol
{
public:
    ProtoCopyOver(QObject *parent = 0);

    void handle(QString data, Management *management, QTcpSocket *masterSocket);

private:
    /**
     * @brief A function that will be invoked when 'CopyBuildOver' has been found in ReadyCheck
     * @param data The data that will be parsed
     */
    void CopyBuildOver(QString data, Management* management, QTcpSocket *masterSocket);
};

#endif // PROTOCOPYOVER_H
