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
#include "xmlWriter.h"
#include "Slave.h"
#include "Machine.h"
#include "server.h"
#include "myDirIterator.h"
#include "protocolhandler.h"
#include "database.h"
#include "buildmd5.h"
#include "simulation.h"

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
     * \fn void slaveGotBuild(Machine *machine, int BuildID,  QString buildName, bool buildExists);
     * @brief A signal that will invoke a function on the mainform to add the build to the view
     * @param machine The machine on which the build must be displayed
     * @param BuildID The ID of the build that must be added
     * @param buildExists A boolean value indicating whether the build on slave exist on master
     * @param buildName The build name that is on the slave machine side
     */
    void slaveGotBuild(Machine *machine, int BuildID,  QString buildName, bool buildExists);

    /**
     * \fn void slaveBuildSizeSame(int buildId, int slaveId, bool isTheSame);
     * @brief Signal emitted which says if the build is the same on the master and the slave
     * @param buildId The ID of the build is the same or not
     * @param slaveId The ID of the machine which has the build
     * @param isTheSame boolean indicating if it is the same or not
     */
    void slaveBuildSizeSame(int buildId, int slaveId, bool isTheSame);

    /**
     * \fn void slaveBuildSynched(int machineId, int buildId, double percentageSynched);
     * @brief Signal emitted which indicates how far the build is synchronised
     * @param buildId The ID of the build is the same or not
     * @param slaveId The ID of the machine which has the build
     * @param percentageSynched The percentage that the build is synchronised
     */
    void slaveBuildSynched(int machineId, int buildId, double percentageSynched);

    /**
     * \fn void newSlaveUpdatedBuildName(int machineID, int buildID, QString buildName);
     * @brief A signal emitted each time a slave has updated their build name to a new name
     * @param machineID The machine ID on which the updated buildName is done
     * @param buildID The ID of the build was updated
     * @param buildName The build name that was updated
     */
    void newSlaveUpdatedBuildName(int machineID, int buildID, QString buildName);

    /**
     * \fn void slaveDeletedBuild(int machineID, int buildID);
     * @brief A signal emitted each time a slave deleted a build sucessfully on their side
     * @param machineID The machine ID on which the build deletion took place
     * @param buildID The ID of the build was deleted
     */
    void slaveDeletedBuild(int machineID, int buildID);

    /**
      * \fn void buildDeleted();
      * @brief Emitted when the build has been removed sucessfully from the master and all machines are notified to delete the build
      */
    void buildDeleted();
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
     * @fn Build *getBuildByID(int id);
     * @brief returns the build by ID from the Management object or 0 if it does not exist
     * @return buildToRetrieve
     */
    Build *getBuildByID(int id);

    /**
     * @fn QList<Build *> getAllBuilds()
     * @brief returns all the builds in the Management object
     * @return allBuilds
     */
    QList<Build *> getAllBuilds() {return allBuilds;}

    /**
    * \fn int getBuildCount();
    * @brief The function that accesses the buildCount variable
    * @return buildCount
    */
    int getBuildCount() {return allBuilds.size();}

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
    * \fn void addBuild(Build *buildToAdd);
    * @brief The function that adds a Build to the management
    * @param buildToAdd The Build that is to be added
    */
    void addBuild(Build *buildToAdd);

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
     * \fn Build *getBuildByName(QString name);
     * @brief A function that will search for a build and return it by name
     * @param name The name of the build
     * @return The function returns a build if it exist with that name or 0 if it does not
     */
    Build *getBuildByName(QString name);

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
     * \fn bool machineWithBuild(int machineID, int buildID);
     * @brief This will check whether there already exist a certain build on a machine
     * @param machineID The unique Id of the machine
     * @param buildID The unique ID of the build
     * @return Returns true if there is already a build with that ID on the machine with ID specified
     */
    bool machineWithBuild(int machineID, int buildID);

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

    /**
    * \fn void slaveBuildSize(int BuildID, QString buildMD5Value, int slaveId);
    * @brief A function to compare the md5 value of the master machine to that of the slave machine
    * @param BuildID The ID of the build that has the md5 sum value
    * @param buildMD5Value The md5 sum value of the build
    * @param slaveId The ID of the machine which invoked the function
    */
    void slaveBuildSize(int BuildID, QString buildMD5Value, int slaveId);

    /**
    * \fn void slaveABuildSize(int BuildID, QString buildMD5Value, int slaveId);
    * @brief A function which invokes a single build size and which will invoke the rest of the syncrhonisation process
    * @param BuildID The ID of the build that has the md5 sum value
    * @param buildMD5Value The md5 sum value of the build
    * @param slaveId The ID of the machine which invoked the function
    */
    void slaveABuildSize(int BuildID, QString buildMD5Value, int slaveId);

    /**
    * \fn void slaveABuildSize(int BuildID, QString buildMD5Value, int slaveId);
    * @brief A function setting whether the build on a slave is the same or not and emitting the change
    * @param isSame A boolean indicating if it is the same or not
    * @param buildID The ID of the build which isSame value is being set for
    * @param machineId The ID of the machine which invoked the function
    */
    void setSlaveBuildIsSame(bool isSame, int machineId, int buildID);

    QString getMinStats(){return minStats;}

    void setMinStats(QString stats);

    QString getDetStats(){return detStats;}

    void setDetStats(QString stats);

    void machineBuildSynched(int machineId, int buildId, double percentageSynched);

    /**
     * \fn void clearMachines();
     * @brief A function executed after a slave has updated build information
     * @param buildID The ID of the build that has updated on slave machine
     * @param buildName The name of the build that was updated
     * @param machineID The Machine's ID that has an updated buildName
     */
    void slaveUpdatedBuildName(int machineID, int buildID, QString buildName);


    /**
     * \fn void setBuildDirectory(int buildID, QString value);
     * @brief The function to set the build directory to the new information passed through
     * @param buildID The ID of the build that will be updated
     * @param value The value to which the build directory will be updated to
     */
    void setBuildDirectory(int buildID, QString value);

    /**
     * \fn void setBuildName(int buildID, QString value);
     * @brief The function to set the build name to the new information passed through
     * @param buildID The ID of the build that will be updated
     * @param value The value to which the build name will be updated to
     */
    void setBuildName(int buildID, QString value);

    /**
     * \fn void setBuildNumber(int buildID, QString value);
     * @brief The function to set the build number to the new information passed through
     * @param buildID The ID of the build that will be updated
     * @param value The value to which the build number will be updated to
     */
    void setBuildNumber(int buildID, QString value);

    /**
     * \fn void setBuildDescription(int buildID, QString value);
     * @brief The function to set the build description to the new information passed through
     * @param buildID The ID of the build that will be updated
     * @param value The value to which the build description will be updated to
     */
    void setBuildDescription(int buildID, QString value);

    /**
     * \fn void generateUniqueId();
     * @brief A function to generate a unique id for a new connecting machine
     * @return Returns a new unique Id for a new connecting machine
     */
    int generateUniqueId();


    void addSimulation(Simulation * sim);
    int getSimCount() { return simCount; }
    QVector<Simulation*> getAllSims() { return allSimulations; }

    /**
     * \fn void deleteBuild(int buildID);
     * @brief Deletes the build on the master machine and notifies the slave machines of this deletion
     * @param biuldID The ID of the build that will be deleted
     */
    void deleteBuild(int buildID);

    /**
      * void slaveSuccessDeleteABuild(int machineID, int buildID);
      * @brief When there exist a build on the slave which has been sucessfully removed
      * @param machineID The machine which notified of the sucessfull deletion
      * @param buildID The build ID which was deleted on the machine
      */
    void slaveSuccessDeleteABuild(int machineID, int buildID);

private:
    /**
    * \fn void removeBuildLogically(int buildID);
    * @brief Function which logcially removes the build from existence(i.e. from the allBuild array)
    * @param buildID ID of the build to be removed
    */
    void removeBuildLogically(int buildID);
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
    QList<Build *> allBuilds;

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


    int simCount;
    QVector<Simulation*> allSimulations;

    QMutex buildLock;

};

#endif // MANAGEMENT_H
