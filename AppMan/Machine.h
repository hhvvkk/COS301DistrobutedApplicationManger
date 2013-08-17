/**
* @file Machine.h
* @brief This is the structure for the Machine
*/
#ifndef MACHINE_H
#define MACHINE_H

#include <QString>

#include "Build.h"
#include "serverthread.h"

class ServerThread;

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
        ServerThread *serverThread;
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
        ~Machine();

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
        * \fn void setServerThread(ServerThread *sVThread);
        * @brief A function that will be used to set a serverthread that will be used to access the thread
        * @param sVThread the thread of the client connected
        */
        void setServerThread(ServerThread *sVThread);

        /**
        * \fn void copyBuildOver(Build build) = 0;
        * @brief The function that will be overridden by the Slave class to initiate a copy of a build
        */
        virtual void copyBuildOver(Build build) = 0;
};

#endif // MACHINE_H
