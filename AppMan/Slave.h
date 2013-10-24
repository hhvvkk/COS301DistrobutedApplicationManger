/**
* @file Slave.h
* @brief This is the structure for the Slave
*/

#ifndef Slave_H
#define Slave_H

#include <QString>

#include "Build.h"
#include "Machine.h"
#include "protocolhandler.h"

/**
* @class Slave
* @brief This is the structure for the Slave
*/
class Slave : public Machine{
private:
        /**
        * @var slaveBuilds
        * @brief an array of Builds
        */
        Build * slaveBuilds;
        /**
        * @var buildCount
        * @brief a counter for the Builds
        */
        int buildCount;


        /**
        * @var protocolHandler
        * @brief The protocolHandler to send data via the protocols
        */
        ProtocolHandler *protocolHandler;
public:
        /**
        * \fn Slave(int id, QString ip);
        * @brief The parameterised constructor
        * @param id the machineID to assign
        * @param ip the machineIP to assign
        */
        Slave(int id, QString ip);

        /**
        * \fn ~Slave();
        * @brief The destructor
        */
        ~Slave();

        /**
        * \fn Slave();
        * @brief The default constructor
        */
        Slave();

        /**
        * \fn Slave(Slave &toCopy);
        * @brief The copy constructor
        * @param &toCopy The Slave object to copy
        */
        Slave(Slave &toCopy);

        /**
        * \fn void deleteBuild(int id);
        * @brief The overridden deleteBuild function
        * @param id The Build ID
        */
        void deleteBuild(int id);

        /**
        * \fn void addBuild(Build buildToAdd);
        * @brief The overridden addBuild function
        * @param buildToAdd The Build to add
        */
        void addBuild(Build buildToAdd);

        /**
        * \fn void setProtocolHandler(ProtocolHandler *handler);
        * @brief The function to set the protocolHandler for a machine
        * @param handler The ProtocolHandler to set
        */
        void setProtocolHandler(ProtocolHandler *handler);

        /**
        * \fn copyBuildOver(Build b);
        * @brief The overridden copyBuildOver function
        * @param buildToAdd The Build to add
        */
        void copyBuildOver(Build b);

        /**
        * \fn Build* getBuilds() = 0;
        * @brief This function will return all builds related to this slave
        * @return Returns all builds for this machine
        */
        Build *getBuilds();

        /**
        * \fn int getBuildCount();
        * @brief The function to return the amount of builds on the machine
        * @return Returns buildCount
        */
        int getBuildCount();

        void setBuildSame(bool isSame, int buildID);

        void getMinStats();

        void getDetStats();

        /**
        * \fn void slaveABuildSizeDone(int buildID);
        * @brief Function to notify its protocolHandler on a changed build
        */
        void slaveABuildSizeDone();

        /**
        * \fn void updateBuildName(int buildID, QString newBuildName);
        * @brief The function to update the build name on the slave machine
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
        * @brief The function to delete the build physically from the slave machine
        * @param BuildID The ID of the build that will be deleted physically
        */
        void deleteBuildNotify(int BuildID);

        /**
        * \fn void resynchAll();
        * @brief Manually resynchronise all the builds on the machine
        */
        void resynchAll();
};

#endif // Slave_H
