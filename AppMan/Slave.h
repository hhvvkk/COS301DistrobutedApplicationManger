/**
* @file Slave.h
* @brief This is the structure for the Slave
*/

#ifndef Slave_H
#define Slave_H

#include <QString>

#include "Build.h"
#include "Machine.h"
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
        * @var serverThread
        * @brief The serverthread on which this machine runs
        */
        ServerThread *serverThread;
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
        * \fn void deleteBuild(QString name);
        * @brief The overridden deleteBuild function
        * @param name The Build name
        */
        void deleteBuild(QString name);

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
        * \fn void setServerThread(ServerThread *sVThread);
        * @brief A function that will be used to set a serverthread that will be used to access the thread
        * @param sVThread the thread of the client connected
        */
        void setServerThread(ServerThread *sVThread);

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

};

#endif // Slave_H
