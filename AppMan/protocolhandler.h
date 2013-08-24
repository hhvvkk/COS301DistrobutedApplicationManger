#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>

#include "protoslavecurrentbuilds.h"
#include "protocopyover.h"
#include "protosizecheckbuilds.h"
#include "protoconnect.h"
#include "protogetsysinfo.h"


class Management;
class Machine;

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    ////////////////////REQUESTS BY OTHER INTERVENTION////////////////
    explicit ProtocolHandler(Management *man, QObject *parent = 0);

    void setSocket(QTcpSocket *socket);

    void handle(QString data);

    void setMachine(Machine *m);

    ////////////////////REQUESTS BY USER INTERACTION////////////////
    /**
     * \fn void copyBuildOver(int buildId, QString buildName);
     * @brief A function that will initiate a copy over from master to slave machine
     * @param buildId The new id to send to the machine
     * @param buildName The new name to send to the machine
     */
    void copyBuildOver(int buildId, QString buildName);

    void disconnectMachine();


private:
    void requestHandler(QString data);

private:
    Protocol *slaveCurrentBuilds;
    Protocol *copyOver;
    Protocol *connect;
    Protocol *sizeCheckBuilds;
    Protocol *getSysInfo;

    Management *management;

    bool firstTalk;

    Machine *machine;

    QTcpSocket *slaveSocket;

};

#endif // PROTOCOLHANDLER_H
