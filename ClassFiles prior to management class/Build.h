/**
* @file Build.h
* @brief This is the structure for the Build
* @author 101Solutions
* @version 2.0
*/
#ifndef BUILD_H
#define BUILD_H

#include <string>

using namespace std;
/**
* @class Build
* @brief This is the structure for the various builds
*/
class Build{
	private:
		/**
		* @var buildID
		* @brief the ID of the Build
		*/
		int buildID;
		/**
		* @var buildName 
		* @brief the name of the Build
		*/
		string buildName;
		/**
		* @var buildDescription
		* @brief the Description of the Build
		*/
		string buildDescription;
	public:
		/**
		* \fn Build(int id, string name, string descript);
		* @brief The parameterised constructor
		* @param id The buildID to assign
		* @param name The buildName to assign
		* @param descript The buildDescription to assign
		*/
		Build(int id, string name, string descript);
		/**
		* \fn ~Build();
		*@brief The destructor
		*/
		~Build();
		/**
		* \fn Build();
		*@brief The default constructor
		*/
		Build();
		/**
		* \fn void setBuild(int id, string name, string descript);
		* @brief The function to set build details
		* @param id The buildID to assign
		* @param name The buildName to assign
		* @param descript The buildDescription to assign
		*/
		void setBuild(int id, string name, string descript);
		/**
		* \fn int getBuildID() {return buildID;};
		* @brief The function to access the buildID variable
		* @return buildID
		*/
		int getBuildID() {return buildID;};
		/**
		* \fn string getBuildName() {return buildName;};
		* @brief The function to access the buildName variable
		* @return buildName
		*/
		string getBuildName() {return buildName;};
		/**
		* \fn string getBuildDescription() {return buildDescription;};
		* @brief The function to access the buildDescription variable
		* @return buildDescription
		*/
		string getBuildDescription() {return buildDescription;};
		/**
		* \fn void setBuildID(int id);
		* @brief The function to mutate the buildID variable
		* @param id The ID to be assigned to BuildID
		*/
		void setBuildID(int id);
		/**
		* \fn void setBuildName(string name);
		* @brief The function to mutate the buildName variable
		* @param name The name to be assigned to buildName
		*/
		void setBuildName(string name);
		/**
		* \fn void setBuildDescription(string descript);
		* @brief The function to mutate the buildDescription variable
		* @param descript The description to be assigned tobuildDescription
		*/
		void setBuildDescription(string descript);
		/**
		* \fn void printBuild();
		*@brief The function that prints out this Build
		*/
		void printBuild();
};


#endif // BUILD_H