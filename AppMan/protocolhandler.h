#ifndef PROTOCOLHANDLER_H
#define PROTOCOLHANDLER_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QVariantMap>
#include <QVariant>
#include <QFuture>

#include "json.h"

#include "protoslavecurrentbuilds.h"
#include "protocopyover.h"
#include "protosizecheckbuilds.h"
#include "protoconnect.h"
#include "protogetsysinfo.h"
#include "protosendbuild.h"
#include "protoupdatebuildinfo.h"
#include "protoupdatemachineinfo.h"
#include "protodeletebuild.h"


class Management;
class Machine;

class ProtocolHandler : public QObject
{
    Q_OBJECT
public:
    ////////////////////REQUESTS BY OTHER INTERVENTION////////////////
    explicit ProtocolHandler(Management *man, QObject *parent = 0);

    ~ProtocolHandler();

    /**
     * \fn void setSocket(QTcpSocket *socket);
     * @brief A function that will set the socket for later use
     * @param socket The client socket to which will be written
     */
    void setSocket(QTcpSocket *socket);



    /**
     * \fn void handle(QString data);
     * @brief A function which handles the incoming data accordingly
     * @param data The data that will be used and parsed
     */
    void handle(QString data);


    /**
     * \fn QStringList splitRequests(QString data);
     * @brief A function which strips the slashes from data and splits various requests that may be together into separate requests and places those request in the return object
     * @param data The data that will be used and parsed
     * @return The different requests that will be returned inside an object
     */
    QStringList splitRequests(QString data);

    /**
     * \fn void setMachine(Machine *m);
     * @brief setMachine will set the machine of this protcolHandler so that backwards communication is possible
     * @param m The machine to set so that a link backwards is possible
     */
    void setMachine(Machine *m);

    /**
     * \fn Machine* ProtocolHandler::getMachine();
     * @brief This function will return the machine variable that is set
     * @return Returns the machine that is set
     */
    Machine* getMachine();

    ////////////////////REQUESTS BY USER INTERACTION////////////////
    /**
     * \fn void copyBuildOver(int buildId, QString buildName);
     * @brief A function that will initiate a copy over from master to slave machine
     * @param buildId The new id to send to the machine
     * @param buildName The new name to send to the machine
     */
    void copyBuildOver(int buildId, QString buildName);

    /**
     * \fn void disconnectMachine();
     * @brief The disconnect function that will call the Connect Protocol disconnect
     */
    void disconnectMachine();

    /**
     * \fn void disconnectMachine();
     * @brief The disconnect function that will call the Connect Protocol disconnect
     */
    void getMinStats();
    /**
     * \fn void disconnectMachine();
     * @brief The disconnect function that will call the Connect Protocol disconnect
     */
    void getDetStats();

    /**
    * \fn void slaveABuildSizeDone(int buildID);
    * @brief A function to check for file differences of a changed build
    */
    void slaveABuildSizeDone();



    /**
    * \fn void recheckAllSizes();
    * @brief A function executing each time the master machine loops through the builds that are different. This happens by calling this function from ProtoSendBuild
    */
    void recheckAllSizes();

    /**
    * \fn void updateBuildName(QString newBuildName);
    * @brief A function to update the build name of the build on the slave machine
    * @param newBuildName The new build build name to update it to
    * @param buildID the build ID of the build to be updated
    */
    void updateBuildName(int buildID, QString newBuildName);


    /**
    * \fn void updateUniqueID(int uniqueID);
    * @brief The function to update the unique ID on the machine
    * @param uniqueID the new ID of the machine
    */
    void updateUniqueID(int uniqueID);


    /**
    * \fn void deleteBuildNotify(int BuildID);
    * @brief The function to notify the slave to delete the build physically
    * @param BuildID The ID of the build that will be deleted physically
    */
    void deleteBuildFromSlave(int BuildID);

private:
    /**
     * \fn void requestHandler(QString data);
     * @brief This function will be invoked by the handle function after the data string has been parsed to remove slashes. The function will invoke a protocol depending on what has been found
     * @param data The data on which further parsing will be done
     */
    void requestHandler(const QString data);

private:
    /**
     * @var slaveCurrentBuilds The protocol SlaveCurrentBuilds
     */
    Protocol *slaveCurrentBuilds;

    /**
     * @var copyOver The protocol CopyOver
     */
    Protocol *copyOver;

    /**
     * @var connect The protocol Connect
     */
    Protocol *connect;

    /**
     * @var sizeCheckBuilds The protocol SizeCheckBuilds
     */
    Protocol *sizeCheckBuilds;

    /**
     * @var getSysInfo The protocol GetSysInfo
     */
    Protocol *getSysInfo;

    /**
     * @var getSysInfo The protocol SendBuild
     */
    Protocol *sendBuild;


    /**
     * @var updateBuildInfo The protocol UpdateBuildInfo
     */
    Protocol *updateBuildInfo;


    /**
     * @var updateMachineInfo The protocol Update machine info
     */
    Protocol *updateMachineInfo;

    /**
     * @var deleteBuild The protocol Delete Build
     */
    Protocol *deleteBuild;


    /**
     * @var management A management pointer so that it may be used directly
     */
    Management *management;

    /**
     * @var firstTalk a variable that will determine if it is the first time data has been read
     */
    bool firstTalk;

    /**
     * @var machine the variable back to machine to be used
     */
    Machine *machine;

    /**
     * @var slaveSocket The slave machine socket
     */
    QTcpSocket *slaveSocket;

};

#endif // PROTOCOLHANDLER_H
