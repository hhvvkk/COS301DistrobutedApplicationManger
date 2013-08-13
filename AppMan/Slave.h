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
        * \fn virtual void deleteBuild(QString name);
		* @brief The overridden deleteBuild function
		* @param name The Build name
		*/
        virtual void deleteBuild(QString name);
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

};

#endif // Slave_H
