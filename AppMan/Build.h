/**
* @file Build.h
* @brief This is the structure for the Build
*/

#ifndef BUILD_H
#define BUILD_H

#include <QString>
#include <QMutex>
#include <QFuture>
#include <QtConcurrentRun>
#include "buildmd5.h"

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
		QString buildName;
		/**
		* @var buildDescription
		* @brief the Description of the Build
		*/
		QString buildDescription;
        /**
          * @var buildDirectory
          * @brief the Diretory of the Build
          */
        QString buildDirectory;

        /**
        * @var isSame
        * @brief This is a flag indicating if there is a difference between the master build and slave build(purely used in slave builds)
        */
        bool isSame;

        /**
        * @var buildMD5
        * @brief The buildMD5 class to be used by all other machines, such that they all do not recreate this class
        */
        BuildMD5 *buildMD5;

        ///lock for the buildMD5Class
        QMutex lock;
    public:
		/**
        * \fn Build(int id, QString name, QString descript, QString direc);
		* @brief The parameterised constructor
		* @param id The buildID to assign
		* @param name The buildName to assign
		* @param descript The buildDescription to assign
        * @param direct The buildDirectory to assign
		*/
        Build(int id, QString name, QString descript, QString direct);
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
        * \fn void setBuild(int id, QString name, QString descript, QString direc);
		* @brief The function to set build details
		* @param id The buildID to assign
		* @param name The buildName to assign
		* @param descript The buildDescription to assign
		*/
        void setBuild(int id, QString name, QString descript, QString direc);
		/**
        * \fn int getBuildID() {return buildID;}
		* @brief The function to access the buildID variable
		* @return buildID
		*/
        int getBuildID() {return buildID;}
		/**
        * \fn QString getBuildName() {return buildName;}
		* @brief The function to access the buildName variable
		* @return buildName
		*/
        QString getBuildName() {return buildName;}
		/**
        * \fn QString getBuildDescription() {return buildDescription;}
		* @brief The function to access the buildDescription variable
		* @return buildDescription
		*/
        QString getBuildDescription() {return buildDescription;}
        /**
		* \fn QString getBuildDirectory() {return buildDirectory;}
        * @brief The function to access the buildDirectory variable
        * @return buildDirectory
        */
        QString getBuildDirectory() {return buildDirectory;}
		/**
        * \fn void setBuildID(int id);
		* @brief The function to mutate the buildID variable
		* @param id The ID to be assigned to BuildID
		*/
		void setBuildID(int id);
		/**
        * \fn void setBuildName(QString name);
		* @brief The function to mutate the buildName variable
		* @param name The name to be assigned to buildName
		*/
        void setBuildName(QString name);
		/**
        * \fn void setBuildDescription(QString descript);
		* @brief The function to mutate the buildDescription variable
		* @param descript The description to be assigned tobuildDescription
		*/
        void setBuildDescription(QString descript);
        /**
        * \fn void setBuildDirectory(QString direc);
        * @brief The function to mutate the buildDirectory variable
        * @param descript The description to be assigned to buildDirectory
        */
        void setBuildDirectory(QString direc);

        /**
        * \fn void setBuildDifference(bool same);
        * @brief This function will be used to set whether the build on master is same as the one that the slave got
        * @param diffr Boolean which should be set to false if there is a different and if it is the same should be set to true
        */
        void setIsSame(bool same);

        /**
        * \fn bool getIsSame();
        * @brief This function is used to determine whether a build has different build Md5 values
        * @return Returns true if the build on master and slave are the same and false if its different
        */
        bool getIsSame();

        /**
        * \fn BuildMD5 *getBuildMD5Class();
        * @brief Function to generate buildMD5 class or return it if it exists
        * @return Returns the buildMD5 class for this build which was generated
        */
        BuildMD5 *getBuildMD5Class();


        /**
        * \fn  void setBuildMD5Expired();
        * @brief Sets the variable to notify that the buildMD5 Class  is old and needs to be regenerated
        */
        void setBuildMD5Expired();
private:
        /**
        * \fn void generateBuildMD5();
        * @brief Function to generate the buildmd5 class
        */
        void generateBuildMD5();
};


#endif // BUILD_H
