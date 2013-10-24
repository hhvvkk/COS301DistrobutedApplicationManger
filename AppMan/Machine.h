/**
* @file Machine.h
* @brief This is the structure for the Machine
*/
#ifndef MACHINE_H
#define MACHINE_H

#include <QString>

#include "Build.h"
#include "protocolhandler.h"

/**
* @class Machine
* @brief This is the structure for the various machines
*/
class Machine{
	protected:
        /**
        * @var machineID
        * @brief the ID of the machine
        */
        int machineID;
        /**
        * @var machineIP
        * @brief the IP of the machine
        */
        QString machineIP;
        /**
        * @var minStats
        * @brief the minimal stats of the machine
        */
        QString minStats;
        /**
        * @var detStats
        * @brief the detailed stats of the machine
        */
        QString detStats;

        bool gotStats;
private:
        /**
        * \fn  bool validateIpAddress(const QString &ipAddress);
        * @brief A function validating whether ip address is correct
        * @param ipAddress The ip to be validated
        * @return Returns true if it is a valid ip address
        */
    bool validateIpAddress(const QString &ipAddress);

public:
        /**
        * \fn Machine(int id, QString ip);
        * @brief The parameterised constructor
        * @param id The machineID to assign
        * @param ip The machineIP to assign
        */
        Machine(int id, QString ip);

        /**
        * \fn ~Machine();
        * @brief The destructor
        */
        virtual ~Machine();

        /**
        * \fn Machine();
        * @brief The default constructor
        */
        Machine();

        /**
        * \fn int getMachineID() {return machineID;};
        * @brief The function to access the machineID variable
        * @return machineID
        */
        int getMachineID() {return machineID;}

        /**
        * \fn QString getMachineIP() {return machineIP;};
        * @brief The function to access the machineIP variable
        * @return machineIP
        */
        QString getMachineIP() {return machineIP;}

        /**
        * \fn void setMachineID(int id);
        * @brief The function to mutate the machineID variable
        * @param id The ID to assign to machineID
        */
        void setMachineID(int id);

        /**
        * \fn void setMachineIP(QString ip);
        * @brief The function to mutate the machineIP variable
        * @param ip The IP to be assigned to machineIP
        */
        void setMachineIP(QString ip);

        QString getMinStatsString();
        QString getDetStatsString();

        void setMinStatsString(QString data);
        void setDetStatsString(QString data);

        /**
        * \fn virtual void deleteBuild(int id);
        * @brief The pure virtual deleteBuild function that will be overridden by Slave class which receives the buildId as parameter
        * @param id The Build ID
        */
        virtual void deleteBuild(int id) = 0;

        /**
        * \fn virtual void addBuild(Build buildToAdd);
        * @brief The pure virtual addBuild function that will be overridden by Slave class
        * @param buildToAdd The Build to add
        */
        virtual void addBuild(Build buildToAdd) = 0;

        /**
        * \fn void setServerThread(ServerThread *sVThread);
        * @brief A pure virtual function that will be used to set a serverthread that will be used to access the thread
        * @param sVThread the thread of the client connected
        */
        virtual void setProtocolHandler(ProtocolHandler *handler) = 0;

        /**
        * \fn void copyBuildOver(Build build) = 0;
        * @brief The pure virtual function that will be overridden by the Slave class to initiate a copy of a build
        */
        virtual void copyBuildOver(Build build) = 0;

        /**
        * \fn Build* getBuilds() = 0;
        * @brief The pure virtual function that will be overridden by the Slave class to return all builds
        * @return Returns all builds for this machine. Will be overridden by Slave to return all the builds
        */
        virtual Build* getBuilds() = 0;

        /**
        * \fn int getBuildCount() = 0;
        * @brief The pure virtual function that will be overridden by the Slave class to return build count
        * @return returns the build count
        */
        virtual int getBuildCount() = 0;

        /**
        * \fn void setBuildSame(bool isSame, int buildID) = 0;
        * @brief The pure virtual function that will be overridden by the Slave to set the build size boolean to indicate that it is the same
        * @param isSame The value indicating if it is the same or not(true or false)
        * @param buildID The Id of the build for which it will be set
        */
        virtual void setBuildSame(bool isSame, int buildID) = 0;

        virtual void getDetStats() = 0;

        virtual void getMinStats() = 0;


        /**
        * \fn void slaveABuildSizeDone(int buildID) = 0;
        * @brief The pure virtual function that will be overridden by the Slave to notify its protocolHandler on a changed build
        */
        virtual void slaveABuildSizeDone() = 0;

        /**
        * \fn void updateBuildName(int buildID, QString newBuildName) = 0;
        * @brief The pure virtual function that will be overridden by the Slave to update the build name on slave machine
        * @param newBuildName The new build name to update it to
        * @param buildID the Id of the build to be updated
        */
        virtual void updateBuildName(int buildID, QString newBuildName) = 0;

        /**
        * \fn void updateUniqueID(int uniqueID) = 0;
        * @brief The pure virtual function that will be overridden by the Slave to update the machine unique ID
        * @param uniqueID The new unique ID of the machine
        */
        virtual void updateUniqueID(int uniqueID) = 0;

        /**
        * \fn void deleteBuildNotify(int BuildID) = 0;
        * @brief The pure virtual function that will be overridden by the Slave to physically go and delete the build
        * @param BuildID The ID of the build that will be deleted physically
        */
        virtual void deleteBuildNotify(int BuildID) = 0;

        /**
        * \fn void deleteBuildNotify(int BuildID) = 0;
        * @brief The pure virtual function that will be overridden by the Slave to manually resynch all builds
        */
        virtual void resynchAll() = 0;

};

#endif // MACHINE_H
