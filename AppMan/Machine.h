/**
* @file Machine.h
* @brief This is the structure for the Machine
* @author 101Solutions
* @version 2.0
*/
#ifndef MACHINE_H
#define MACHINE_H

#include <QString>

#include "Build.h"
using namespace std;
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
		* @var machineStatus
		* @brief the status of the machine
		*/
                QString machineStatus;
		/**
		* @var machineType
		* @brief 0 is Master, 1 is Slave
		*/
		bool machineType;
	public:
		/**
        * \fn Machine(int id, QString ip, QString status);
		* @brief The parameterised constructor
		* @param id The machineID to assign
		* @param ip The machineIP to assign
		* @param status The machineStatus to assign
		* @param type The boolean indicating Master or Slave
		*/
        Machine(int id, QString ip, QString status, bool type);
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
                * \fn void setMachine(int id, QString ip, QString status);
		* @brief The function to set machine details
		* @param id The machineID to assign
		* @param ip The machineIP to assign
		* @param status The machineStatus to assign
		* @param type The boolean indicating Master or Slave
		*/
                void setMachine(int id, QString ip, QString status, bool type);
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
                * \fn QString getMachineStatus() {return machineStatus;};
		* @brief The function to access the machineStatus variable
		* @return machineStatus
		*/
                QString getMachineStatus() {return machineStatus;}
		/**
		* \fn bool getMachineType() {return machineType;};
		* @brief The function to access the machineType variable
		* @return machineType
		*/
                bool getMachineType() {return machineType;}
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
                * \fn void setMachineStatus(QString status);
		* @brief The function to mutate the machineStatus variable
		* @param status The status to be assigned to machineStatus
		*/
                void setMachineStatus(QString status);
		/**
		* \fn void setMachineType(bool type);
		* @brief The function to mutate the machineType variable
		* @param type The bool to be assigned to machineType
		*/
		void setMachineType(bool type);
		/**
		* \fn void printMachine();
		*@brief The function to print this Machine
		*/
		void printMachine();
		
		/**
                * \fn virtual void deleteBuild(QString name){};
		* @brief The virtual function that is overridden to delete Builds by name
		* @param name The name of the Build
		*/
                virtual void deleteBuild(QString name){}
		/**
		* \fn virtual void deleteBuild(int id){};
		* @brief The virtual function that is overridden to delete Builds by ID
		* @param id The ID of the Build to be deleted
		*/
                virtual void deleteBuild(int id){}
		
		/**
		* \fn virtual void addBuild(Build buildToAdd){};
		* @brief The virtual function that is overridden to add a Build object
		* @param buildToAdd The Build to be added
		*/
                virtual void addBuild(Build buildToAdd){}
		/**
		* \fn virtual void printBuilds(){};
		* @brief The virtual function that is overridden to print all the Machine builds
		*/
                virtual void printBuilds(){}
			
		/**
                * \fn virtual void deleteSlave(QString ip){};
		* @brief The virtual function that is overridden to delete a Slave by IP
		* @param ip The IP of the Slave to delete
		*/
                virtual void deleteSlave(QString ip){}
		/**
		* \fn virtual void deleteSlave(int id){};
		* @brief The virtual function that is overridden to delete a Slave by ID
		* @param id The ID of the Slave to delete
		*/
                virtual void deleteSlave(int id){}
		/**
		* \fn virtual void addSlave(Machine slaveToAdd){};
		* @brief The virtual function that is overridden to add a Slave object
		* @param slaveToAdd The Slave to be added
		*/
                virtual void addSlave(Machine slaveToAdd){}
		/**
		* \fn virtual void printSlaves(){};
		* @brief The  virtual function that is overridden to print all the Machine slaves
		*/
                virtual void printSlaves(){}
};

#endif // MACHINE_H
