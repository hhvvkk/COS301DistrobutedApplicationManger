/**
* @file management.h
* @brief This is the file containing the client management class
* @author 101Solutions
* @version 2.0
*/

#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QString>

#include "Build.h"
#include "network.h"
#include "xmlReader.h"

/**
 * @class Management
 * @brief The Management class will be used as a facade to the logic behind the application
 */
class Management : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Management The constructor of the management class
     */
    Management();
    /**
      * \fn ~Management();
      * The destructor
      */
    ~Management();

    /**
     * @brief connectToServer function called to connect to server through chain of command from Management to SocketClient
     * @param ipAddress The Ip address on which the server is run
     * @param port The port of the server to connect to
     */
    void connectToServer(QString ipAddress, int port);

    /**
     * @brief disconnectFromServer function that will disconnect from the AppMan Application
     */
    void disconnectFromServer();

    /**
     * @brief writeSomedata a function that writes some data to debug this
     */
    void writeSomedata(){
        network->writeSOme();
    }
    /**
     * \fn void addBuild(Build buildToAdd);
     * @brief The function that adds a Build to the management
     * @param buildToAdd The Build that is to be added
     */
    void addBuild(Build buildToAdd);
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
     * @brief addMyBuilds adds the builds that are in the XML
     */
    void addMyBuilds();

    xmlReader getxRead() {return xRead;}

    Build getBuildByID(int num);
private:
    /**
     * @brief network An Object that will harbour some of the network information
     */
    Network *network;
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

     xmlReader xRead;
};

#endif // MANAGEMENT_H
