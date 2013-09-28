/**
* @file management.h
* @brief This is the facade for the Management
*/

#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QDebug>
#include <QObject>
#include <QVector>
#include <QMutex>
#include <QFile>
#include <QCryptographicHash>

#include "Build.h"
#include "Slave.h"
#include "Machine.h"
#include "server.h"
#include "myDirIterator.h"
#include "protocolhandler.h"

//forward declaration of server so that it can be used
class Server;

/**
* @class Management
* @brief This is the facade for the Management
*/

class Management : public QObject
{
    Q_OBJECT
   //signals to talk to the mainForm("in a sence")
signals:
    /**
     * \fn void newSlaveConnected(Machine *m)
     * @brief newSlaveConnected A signal emitted each time a new slave connects
     */
    void newSlaveConnected(Machine *m);

    /**
     * \fn void slaveDisconnected(int index)
     * @brief slaveDisconnected A signal emitted each time a slave disconnects
     */
    void slaveDisconnected(int index);

    /**
     * \fn void slaveGotBuild(Machine *machine, int BuildID, QString buildName, bool buildExists);
     * @brief A signal that will invoke a function on the mainform to add the build to the view
     * @param machine The machine on which the build must be displayed
     * @param BuildID The number of the build that must be added
     * @param buildExists A boolean value indicating whether the build on slave exist on master
     * @param buildName The build name that is on the slave machine side
     */
    void slaveGotBuild(Machine *machine, int BuildID,  QString buildName, bool buildExists);

    void slaveBuildSizeSame(int buildId, int slaveId, bool isTheSame);

    void slaveBuildSynched(int machineId, int buildId, double percentageSynched);
public:
    /**
    * \fn Management();
    * @brief The default costructor
    */
    explicit Management(QObject *parent = 0);

    /**
    * \fn ~Management();
    * @brief The Destructor
    */
    ~Management();

    /**
     * @fn Build * getBuildByID(int id);
     * @brief returns the build by ID from the Management object
     * @return buildToRetrieve
     */
    Build getBuildByID(int id);

    /**
     * @fn Build * getAllBuilds()
     * @brief returns all the builds in the Management object
     * @return allBuilds
     */
    Build * getAllBuilds() {return allBuilds;}

    /**
    * \fn int getBuildCount();
    * @brief The function that accesses the buildCount variable
    * @return buildCount
    */
    int getBuildCount() {return buildCount;}

    /**
    * \fn vector<Machine*> getAllMachines() {return allMachines;}
    * @brief The function that accesses the allMachines variable
    * @return allMachines
    */
    QVector<Machine*> getAllMachines() {return allMachines;}

    /**
    * \fn int getMachineCount() {return machineCount;}
    * @brief The function that accesses the machineCount variable
    * @return machineCount
    */
    int getMachineCount() {return machineCount;}


    /**
    * \fn void addBuild(Build buildToAdd);
    * @brief The function that adds a Build to the management
    * @param buildToAdd The Build that is to be added
    */
    void addBuild(Build buildToAdd);

    /**
     * \fn void addMachine(QString address, ProtocolHandler handler);
     * @brief addMachine A function adding a machine to the allMachines
     * @param address The machine Ip address to add
     * @param handler the Protocol handler to allow communication to the machine
     */
    void addMachine(int uniqueID, QString address, ProtocolHandler *handler);

    /**
     * \fn void removeMachine(Machine *m);
     * @brief removeMachine A function that will remove the machine sent by reference
     * @param m The machine to be removed from management class
     */
    void removeMachine(Machine *m);

    /**
     * \fn Machine *getMachineAt(int i);
     * @brief getMachineAt returns a machine by reference
     * @param i the index of the machine to return
     * @return Returns a machine at the index from allMachines or NULL if it does not exist
     */
    Machine *getMachineAt(int i);

    /**
     * \fn void setServer(Server *serv);
     * @brief setServer a function to set this class' server
     * @param serv the server to set
     */
    void setServer(Server *serv);

    /**
     * \fn void startServer();
     * @brief startServer The function that starts the server
     */
    void startServer();

    /**
     * \fn void stopServer();
     * @brief stopServer the function that stops the server
     */
    void stopServer();

    /**
     * \fn void setPort(int newPort);
     * @brief setPort Function that sets the port on which the server will be run
     * @param newPort The new port for the server
     */
    void setPort(int newPort);

    /**
     * \fn bool buildExistWithName(QString name);
     * @brief buildExistWithName Will check whether a build exist with a certain name
     * @param name The name of the build
     * @return The function returns true if the build exist with that name
     */
    bool buildExistWithName(QString name);

    /**
     * \fn Build getBuildByName(QString name);
     * @brief A function that will search for a build and return it by name
     * @param name The name of the build
     * @return The function returns a build if it exist with that name
     */
    Build getBuildByName(QString name);

    /**
     * \fn bool machineExistWithIp(QString ip);
     * @brief Will check whether a machine exist with a certain ip
     * @param ip The ip of the machine
     * @return Returns true if machine exist otherwise false
     */
    bool machineExistWithIp(QString ip);

    /**
     * \fn void copyBuildOver(QString ipAddress, QString buildName);
     * @brief Will initiate a copy build over from master to slave
     * @param machineId The uniqueId of the machine
     * @param buildName The buildName to copy over
     */
    void copyBuildOver(int machineId, QString buildName);

    /**
     * \fn void addBuildToSlave(int machineId, QString BuildID);
     * @brief This function will be called once the build has been added to the slave machine. This emits a slaveGotBuild signal
     * @param machineId The unique Id of the machine
     * @param BuildID The buildName to copy over
     */
    void addBuildToSlave(int machineId, int BuildID, QString buildName);

    /**
    * \fn QString getBuildMD5(Build b);
    * @brief This function will calculate the MD5 of a build by calculating the MD5 of the whole directory where build is stored
    * @param build The build on which the MD5 will be calculated
    * @return Returns the MD5 sum of a directory where a build will be
    */
    QString getBuildMD5(Build* build);

    /**
    * \fn Machine *getMachineById(int machineId);
    * @brief This function will run through the machines and find the machine with a certain IP address
    * @param machineIp The IP of the machine to return
    * @return Returns the pointer to a machine with an ip or returns 0 if that machine does not exist anymore
    */
    Machine *getMachineById(int machineId);


    void slaveBuildSize(int BuildID, QString buildMD5Value, int slaveId);

    void slaveABuildSize(int BuildID, QString buildMD5Value, int slaveId);

    void setSlaveBuildIsSame(bool isSame, int machineId, int buildID);

    QString getMinStats(){return minStats;}

    void setMinStats(QString stats);

    QString getDetStats(){return detStats;}

    void setDetStats(QString stats);

    void machineBuildSynched(int machineId, int buildId, double percentageSynched);
private:
    /**
     * \fn void clearMachines();
     * @brief A function to generate a unique id for a new connecting machine
     * @return Returns a new unique Id for a new connecting machine
     */
    int generateUniqueId();

private:
    /**
     * @var server
     * @brief server The server that will be run on which clients connect to
     */
    Server *server;

    /**
    * @var allBuilds
    * @brief an array of Build objects
    */
    Build * allBuilds;

    /**
    * @var buildCount
    * @brief a counter for the Build array
    */
    int buildCount;

    /**
    * @var machineCount
    * @brief a counter for the Machine array
    */
    int machineCount;

    /**
    * @var allMachines
    * @brief a vector of Machine objects
    */
    QVector<Machine*> allMachines;

    /**
     * @var lock
     * @brief A lock that will be used to ensure concurrent events take place in correct order
     */
    QMutex *lock;

    QString minStats;
    QString detStats;

};

#endif // MANAGEMENT_H
