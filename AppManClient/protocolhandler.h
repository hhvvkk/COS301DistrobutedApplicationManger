#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include "protosizecheckbuilds.h"
#include "protoslavecurrentbuilds.h"
#include "protoconnect.h"
#include "protocopyover.h"
#include "protogetsysinfo.h"

class Management;

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(Management *man, QObject *parent = 0);

    void handle(QString data);

    void requestHandler(QString data);

    void disconnectFromMaster();

    void connectToServer(QString IpAddress, int serverPort);

    void setSocket(QTcpSocket *newSocket);

signals:

public slots:

private:
    Management *management;

    QTcpSocket *masterSocket;

    Protocol *connect;
    Protocol *copyOver;
    Protocol *sizeCheckBuilds;
    Protocol *slaveCurrentBuilds;
    Protocol *getSysInfo;

};

#endif // PROTOCOLHANDLER_H
