/**
* @file management.h
* @brief This is the file containing the client management class
*/

#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QString>
#include <QDir>
#include <QCryptographicHash>

#include "Build.h"
#include "network.h"
#include "xmlReader.h"
#include "xmlWriter.h"
#include "buildmd5.h"
#include "myDirIterator.h"

/**
 * @class Management
 * @brief The Management class will be used as a facade to the logic behind the application
 */
class Management : public QObject
{
    Q_OBJECT

signals:
    void signalConnected(bool connectionValue);

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
     * \fn void setConnected(bool connectionValue);
     * @brief A function that will set whether it is online or not
     * @param connectionValue the boolean that determines whether it is online or offline
     */
    void setConnected(bool connectionValue);

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

    /**
     * @fn xmlReader getxRead() {return xRead;}
     * @brief getxRead
     * @return the xmlReader
     */
    xmlReader getxRead() {return xRead;}

    /**
     * \fn Build getBuildByID(int num);
     * @brief The function finds a build by its id and returns it
     * @return Returns a build
     * @param num the id of the build
     */
    Build getBuildByID(int num);

    /**
     * \fn void createBuildDirectory(Build build);
     * @brief This creates a build for a directory
     * @param build the Build to be added
     * @return a Build object with the new directory
     */
    Build createBuildDirectory(Build build);

    /**
    * \fn QString getBuildMD5(Build b);
    * @brief This function will calculate the MD5 of a build by calculating the MD5 of the whole directory where build is stored
    * @param build The build on which the MD5 will be calculated
    * @return Returns the MD5 sum of a directory where a build will be
    */
    QString getBuildMD5(Build build);

    /**
    * \fn BuildMD5 *getBuildMD5Class(Build build);
    * @brief This function will calculate the MD5 of each of the files inside a build
    * @param build The build on which the MD5 file values will be calculated
    * @return Returns a BuildMD5 Class containing MD5 values for each and every file
    */
    BuildMD5 *getBuildMD5Class(Build build);

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

     /**
      * @var xRead
      * @brief xRead is an xmlReader instance
      */
     xmlReader xRead;

     /**
     * @var allBuildsDirectory
     * @brief A directory where all builds will be located
     */
     QString allBuildsDirectory;
};

#endif // MANAGEMENT_H
