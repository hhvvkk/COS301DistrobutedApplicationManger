/**
* @file management.h
* @brief This is the file containing the client management class
*/

#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QString>
#include <QDir>
#include <QCryptographicHash>
#include <QSettings>
#include<QProcess>

#include "Build.h"
#include "network.h"
#include "xmlReader.h"
#include "xmlWriter.h"
#include "buildmd5.h"
#include "myDirIterator.h"
#include "sysInfo.h"
#include "directoryhandler.h"
#include "appxmlreader.h"

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
     * \fn void addBuild(Build *buildToAdd);
     * @brief The function that adds a Build to the management
     * @param buildToAdd The Build that is to be added
     */
    void addBuild(Build *buildToAdd);
    /**
     * @fn Build * getAllBuilds()
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
     * \fn Build *getBuildByID(int num);
     * @brief The function finds a build by its id and returns it
     * @return Returns a build by ID
     * @param id the id of the build
     */
    Build *getBuildByID(int id);

    /**
     * \fn void createBuildDirectory(Build *build);
     * @brief This creates a build for a directory
     * @param build the Build to be added
     * @return a Build object with the new directory
     */
    void createBuildDirectory(Build *build);

    /**
    * \fn QString getBuildMD5(Build *b);
    * @brief This function will calculate the MD5 of a build by calculating the MD5 of the whole directory where build is stored
    * @param build The build on which the MD5 will be calculated
    * @return Returns the MD5 sum of a directory where a build will be
    */
    QString getBuildMD5(Build *build);


    QString getDetSysInfo();

    QString getMinSysInfo();

    /**
    * \fn void updateBuildName(int buildID, QString newBuildName);
    * @brief A function to update the build name on the slave machine
    * @param buildID The build ID of the build that will be changed
    * @param newBuildName the new build name to be changed to
    */
    void updateBuildName(int buildID, QString newBuildName);


    /**
    * \fn void updateUniqueID(int uniqueID);
    * @brief The function to update the unique ID on the machine
    * @param uniqueID the new ID of the machine
    */
    void updateUniqueID(int uniqueID);

    void addToAppList(QString appName, QString appDir);

    QMap<QString,QString> getAppList();

    /**
    * \fn void deleteBuild(int buildID);
    * @brief The function to update the unique ID on the machine
    * @param buildID ID of the build to be deleted
    */
    void deleteBuild(int buildID);

    void runThisSim(QString build, QString arg);

    void readAppList();

    /**
    * \fn void deleteBuild(int buildID);
    * @brief The function to update the unique ID on the machine
    * @param masterDirectoryStructure  A stringlist containing the directory structure on the master machine
    * @param intBuildID The build ID to be duplicated
    */
    void duplicateDirectoryStructure(QStringList masterDirectoryStructure, int intBuildID);
private:
    /**
    * \fn void removeBuildLogically(int buildID);
    * @brief Function which logcially removes the build from existence(i.e. from the allBuild array)
    * @param buildID ID of the build to be removed
    */
    void removeBuildLogically(int buildID);

    /**
    * \fn bool isSubPath(QString onePath, QString subPath);
    * @brief Takes two file paths and looks if one is a subpath of the other
    * @param onePath The one path to be checked if a subpath exist in it
    * @param subPath The subpath which may be in the one path
    * @return Returns true if the onepath has a subpath subPath
    */
    bool isSubPath(QString onePath, QString subPath);

    /**
    * \fn void removeFolderStructure(QString masterValue, QString slaveValue);
    * @brief Removes all the folders from the slave value until it has arrived at the master value
    * @param masterValue The value of the master(path of master)
    * @param slaveValue The value of the slave(path of slave)
    * @param buildPath Where the build is currently at
    */
    void removeFolderStructure(QString buildPath, QString masterValue, QString slaveValue);
private:
    /**
     * @brief network An Object that will harbour some of the network information
     */
    Network *network;
    /**
     * @var allBuilds
     * @brief an array of Build objects
     */
     QList<Build *> allBuilds;

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

     /**
      * @var systeminfo
      *@brief systeminfo is an instance of the sysInfo class
      */
     sysInfo systeminfo;


     QMap<QString,QString> appList;

     QMutex lock;
};

#endif // MANAGEMENT_H
