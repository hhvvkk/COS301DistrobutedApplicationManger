#ifndef PROTOSLAVECURRENTBUILDS_H
#define PROTOSLAVECURRENTBUILDS_H

#include "protocol.h"
#include "Build.h"

class ProtoSlaveCurrentBuilds : public Protocol
{
public:
    ProtoSlaveCurrentBuilds(QObject *parent);
    void handle(QString data, Management *management, QTcpSocket *masterSocket);

private:
    void Rechecker(Management *management, QTcpSocket *masterSocket);

    /**
     * @brief A management link that will allow backwards communication
     */
    int buildIterator;
    /**
     * @brief Will be used to loop through the builds
     */
    Build * allBuilds;
};

#endif // PROTOSLAVECURRENTBUILDS_H
