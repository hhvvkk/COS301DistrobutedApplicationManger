/**
* @file Master.h
* @brief This is the structure for the Master
* @author 101Solutions
* @version 2.0
*/
#ifndef Master_H
#define Master_H

#include "Machine.h"
#include "Build.h"
#include "Slave.h"
#include <string>

using namespace std;
/**
* @class Master
* @brief This is the structure for the Master
*/
class Master : public Machine{
	private:
		/**
		* @var masterBuilds
		* @brief an array of type Build
		*/
		Build * masterBuilds;
		/**
		* @var buildCount
		* @brief a counter for the Build array
		*/
		int buildCount;
		/**
		* @var masterSlaves
		* @brief an array of Slave objects
		*/
		Slave * masterSlaves;
		/**
		* @var slaveCount
		* @brief a counter for the Slave array
		*/
		int slaveCount;
	public:
		/**
		* \fn Master(int id, string ip, string status, bool type);
		* @brief The parameterised constructor
		* @param id the machineID to assign
		* @param ip the machineIP to assign
		* @param status the machineStatus to assign
		* @param type The boolean indicating Master or Slave
		*/
		Master(int id, string ip, string status, bool type);
		/**
		* \fn ~Master();
		* @brief The destructor
		*/
		~Master();
		/**
		* \fn Master();
		* @brief The default constructor
		*/
		Master();
		/**
		* \fn Master(Master &toCopy);
		* @brief The copy constructor
		* @param &toCopy The Master object to copy
		*/
		Master(Master &toCopy);
	
		/**
		* \fn virtual void deleteBuild(string name);
		* @brief The overridden deleteBuild function
		* @param name The build name
		*/
		virtual void deleteBuild(string name);
		/**
		* \fn virtual void deleteBuild(int id);
		* @brief The overridden deleteBuild function
		* @param id The Build ID
		*/
		virtual void deleteBuild(int id);
		/**
		* \fn virtual void addBuild(Build buildToAdd);
		* @brief The overridden addBuild function
		* @param buildToAdd The Build to add
		*/
		virtual void addBuild(Build buildToAdd);
		/**
		* \fn virtual void printBuilds();
		* @brief The overridden printBuilds function
		*/
		virtual void printBuilds();
	
		/**
		* \fn virtual void deleteSlave(string ip);
		* @brief The overridden deleteSlave function
		* @param ip The IP of the Slave to delete
		*/
		virtual void deleteSlave(string ip);
		/**
		* \fn virtual void deleteSlave(int id);
		* @brief The overridden deleteSlave function
		* @param id The ID of the Slave to delete
		*/
		virtual void deleteSlave(int id);
		/**
		* \fn virtual void addSlave(Slave slaveToAdd);
		* @brief The overridden addSlave function
		* @param slaveToAdd The Slave that will be added
		*/
		virtual void addSlave(Machine slaveToAdd);
		/**
		* \fn virtual void printSlaves();
		* @brief The overridden printSlaves fuction
		*/
		virtual void printSlaves();

};

#endif // Master_H