/**
* @file management.h
* @brief This is the facade for the Management
* @author 101Solutions
*/

#ifndef MANAGEMENT_H
#define MANAGEMENT_H


#include <string>
#include "Machine.h"
#include "Master.h"
#include "Slave.h"
#include "Build.h"
#include <vector>

#include <QObject>
#include "server.h"

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
     * \fn newSlaveConnected()
     * @brief newSlaveConnected A signal emitted each time a new slave connects
     */
    void newSlaveConnected();

    /**
     * \fn slaveDisconnected()
     * @brief slaveDisconnected A signal emitted each time a slave disconnects
     */
    void slaveDisconnected();
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
    vector<Machine*> getAllMachines() {return allMachines;}
    /**
    * \fn int getMachineCount() {return machineCount;}
    * @brief The function that accesses the machineCount variable
    * @return machineCount
    */
    int getMachineCount() {return machineCount;}

    /**
    * \fn void addMachine(Master masterToAdd);
    * @brief The function that adds a Master to the management
    * @param masterToAdd The Master that is to be added
    */
    void addMachine(Master masterToAdd);
    /**
    * \fn void addMachine(Slave *slaveToAdd);
    * @brief The function that adds a Slave to the management
    * @param slaveToAdd The Slave that is to be added
    */
    void addMachine(Slave *slaveToAdd);
    /**
    * \fn void addBuild(Build buildToAdd);
    * @brief The function that adds a Build to the management
    * @param buildToAdd The Build that is to be added
    */
    void addBuild(Build buildToAdd);

    /**
     * \fn void addMachine(Machine* machine);
     * @brief addMachine A function adding a machine to the allMachines
     * @param machine The machine to add
     */
    void addMachine(Machine* machine);

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
     * \fn void setSlaveOffline(Machine *m, bool isOffline);
     * @brief setSlaveOffline The function that can change the machine's status from online to offline and vice versa
     * @param m The machine on which the status will be changed
     * @param isOffline A boolean to indicate whether the machine should be set online or offline
     */
    void setSlaveOffline(Machine *m, bool isOffline);
signals:

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
    vector<Machine*> allMachines;

};

#endif // MANAGEMENT_H
