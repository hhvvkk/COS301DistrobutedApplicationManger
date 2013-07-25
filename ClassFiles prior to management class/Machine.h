/**
* @file Machine.h
* @brief This is the structure for the Machine
* @author 101Solutions
* @version 2.0
*/
#ifndef MACHINE_H
#define MACHINE_H

#include <string>

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
		string machineIP;
		/**
		* @var machineStatus
		* @brief the status of the machine
		*/
		string machineStatus;
	public:
		/**
		* \fn Machine(int id, string ip, string status);
		* @brief The parameterised constructor
		* @param id The machineID to assign
		* @param ip The machineIP to assign
		* @param status The machineStatus to assign
		*/
		Machine(int id, string ip, string status);
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
		* \fn void setMachine(int id, string ip, string status);
		* @brief The function to set machine details
		* @param id The machineID to assign
		* @param ip The machineIP to assign
		* @param status The machineStatus to assign
		*/
		void setMachine(int id, string ip, string status);
		/**
		* \fn int getMachineID() {return machineID;};
		* @brief The function to access the machineID variable
		* @return machineID
		*/
		int getMachineID() {return machineID;};
		/**
		* \fn string getMachineIP() {return machineIP;};
		* @brief The function to access the machineIP variable
		* @return machineIP
		*/
		string getMachineIP() {return machineIP;};
		/**
		* \fn string getMachineStatus() {return machineStatus;};
		* @brief The function to access the machineStatus variable
		* @return machineStatus
		*/
		string getMachineStatus() {return machineStatus;};
		/**
		* \fn void setMachineID(int id);
		* @brief The function to mutate the machineID variable
		* @param id The ID to assign to machineID
		*/
		void setMachineID(int id);
		/**
		* \fn void setMachineIP(string ip);
		* @brief The function to mutate the machineIP variable
		* @param ip The IP to be assigned to machineIP 
		*/
		void setMachineIP(string ip);
		/**
		* \fn void setMachineStatus(string status);
		* @brief The function to mutate the machineStatus variable
		* @param status The status to be assigned to machineStatus
		*/
		void setMachineStatus(string status);
		/**
		* \fn void printMachine();
		*@brief The function to print this Machine
		*/
		void printMachine();
		
		/**
		* \fn virtual void deleteBuild(string name){};
		* @brief The virtual function that is overridden to delete Builds by name
		* @param name The name of the Build
		*/
		virtual void deleteBuild(string name){};
		/**
		* \fn virtual void deleteBuild(int id){};
		* @brief The virtual function that is overridden to delete Builds by ID
		* @param id The ID of the Build to be deleted
		*/
		virtual void deleteBuild(int id){};
		
		/**
		* \fn virtual void addBuild(Build buildToAdd){};
		* @brief The virtual function that is overridden to add a Build object
		* @param buildToAdd The Build to be added
		*/
		virtual void addBuild(Build buildToAdd){};
		/**
		* \fn virtual void printBuilds(){};
		* @brief The virtual function that is overridden to print all the Machine builds
		*/
		virtual void printBuilds(){};
			
		/**
		* \fn virtual void deleteSlave(string ip){};
		* @brief The virtual function that is overridden to delete a Slave by IP
		* @param ip The IP of the Slave to delete
		*/
		virtual void deleteSlave(string ip){};
		/**
		* \fn virtual void deleteSlave(int id){};
		* @brief The virtual function that is overridden to delete a Slave by ID
		* @param id The ID of the Slave to delete
		*/
		virtual void deleteSlave(int id){};
		/**
		* \fn virtual void addSlave(Machine slaveToAdd){};
		* @brief The virtual function that is overridden to add a Slave object
		* @param slaveToAdd The Slave to be added
		*/
		virtual void addSlave(Machine slaveToAdd){};
		/**
		* \fn virtual void printSlaves(){};
		* @brief The  virtual function that is overridden to print all the Machine slaves
		*/
		virtual void printSlaves(){};
};

#endif // MACHINE_H