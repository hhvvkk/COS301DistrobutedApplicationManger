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
        * \fn void setMachine(int id, QString ip, bool onL);
        * @brief The function to set machine details
        * @param id The machineID to assign
        * param ip The machineIP to assign
        */
        void setMachine(int id, QString ip);

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

        /**
        * \fn virtual void deleteBuild(QString name);
        * @brief The pure virtual deleteBuild function that will be overridden by Slave class which receives buildName as parameter
        * @param name The Build name
        */
        virtual void deleteBuild(QString name) = 0;

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

        virtual void setBuildSame(bool isSame, int buildID) = 0;

        virtual void getDetStats() = 0;

        virtual void getMinStats() = 0;


        /**
        * \fn void slaveABuildSizeDone(int buildID) = 0;
        * @brief The pure virtual function that will be overridden by the Slave to notify its protocolHandler on a changed build
        * @param buildID the build id for which the slave build size are done checking
        */
        virtual void slaveABuildSizeDone(int buildID) = 0;
};

#endif // MACHINE_H
