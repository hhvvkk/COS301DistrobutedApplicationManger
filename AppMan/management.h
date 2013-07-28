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

    Build * getAllBuilds() {return allBuilds;}
    /**
    * \fn int getBuildCount() {return buildCount;}
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
    * \fn void addMachine(Slave slaveToAdd);
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

    void addMachine(Machine* machine);
    void removeMachine(Machine *m);
    void setServer(Server *serv);
    void startServer();
    void stopServer();
    void setPort(int newPort);
signals:

private:
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
