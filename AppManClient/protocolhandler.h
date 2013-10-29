#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QVariantMap>
#include <QVariant>
#include <QFuture>
#include <QtConcurrentRun>

#include "protosizecheckbuilds.h"
#include "protoslavecurrentbuilds.h"
#include "protoconnect.h"
#include "protocopyover.h"
#include "protogetsysinfo.h"
#include "protosendbuild.h"
#include "protoupdatebuildinfo.h"
#include "protoupdatemachineinfo.h"
#include "protodeletebuild.h"
#include "protorunsim.h"
#include "protoapplist.h"
#include "protosendstructure.h"

#include "json.h"


class Management;


/**
  * @class ProtocolHandler
  * @brief The ProtocolHandler class will be responsible for all protocol requests for communication between this(the slave machine) and master machine
  */
class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    explicit ProtocolHandler(Management *man, QObject *parent = 0);



    ~ProtocolHandler();


    /**
     * \fn void handle(QString data);
     * @brief A function which handles the incoming data accordingly
     * @param data The data that will be used and parsed
     */
    void handle(QString data);


    /**
     * \fn QStringList* splitRequests(QString data);
     * @brief A function which strips the slashes from data and splits various requests that may be together into separate requests and places those request in the return object
     * @param data The data that will be used and parsed
     * @return The different requests that will be returned inside an object
     */
    QStringList splitRequests(QString data);

    /**
     * \fn void disconnectFromMaster();
     * @brief A function to disconnect from the master machine
     */
    void disconnectFromMaster();

    /**
     * \fn void connectToServer(QString IpAddress, int serverPort);
     * @brief This function will be used to connect to the master
     * @param ipAddress The ip address of the machine to connect to
     * @param serverPort The port on which the server to connect to will be
     */
    void connectToServer(QString IpAddress, int serverPort);

    /**
     * \fn void setSocket(QTcpSocket *newSocket);
     * @brief This function will set the socket of this class to the specified socket
     * @param socket The new master machine socket
     */
    void setSocket(QTcpSocket *newSocket);

private:
    /**
     * \fn void requestHandler(QString data);
     * @brief This function will be invoked by the handle function after the data string has been parsed to remove slashes. The function will invoke a protocol depending on what has been found
     * @param data The data on which further parsing will be done
     */
    void requestHandler(QString data);

private:
    Management *management;

    QTcpSocket *masterSocket;

    Protocol *connect;
    Protocol *copyOver;
    Protocol *sizeCheckBuilds;
    Protocol *slaveCurrentBuilds;
    Protocol *getSysInfo;
    Protocol *sendBuild;
    Protocol *updateBuildInfo;
    Protocol *updateMachineInfo;
    Protocol *deleteBuild;
    Protocol *aRunSim;
    Protocol *appList;
    Protocol *sendStructure;

};

#endif // PROTOCOLHANDLER_H
