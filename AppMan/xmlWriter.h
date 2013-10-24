/**
* @file xmlWriter.h
* @brief This is the file containing the xmlWriter to store builds
*/
#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QDebug>
#include <QFile>
#include <QMap>
#include <QMapIterator>
#include <QString>
#include <QXmlStreamWriter>
#include <iterator>

#include "xmlReader.h"

/**
 * @class xmlWriter
 * @brief The xmlWriter class will be the class that is used to generate and update the xml of Builds
 */
class xmlWriter
{

public:

    /**
     * @fn xmlWriter();
     * @brief The constructor which creates a reader too....
     */
    xmlWriter();

    /**
     * @fn ~xmlWriter();
     * @brief The deault destructor
     */
    ~xmlWriter();

    /**
     * @fn createXMLFile();
     * @brief The function to create and append to the XML file
     */
    void createXMLFile();


    /**
     * @fn void receiveBuild(QString num,QString name,QString descript, QString direc);
     * @brief The function that accepts parameters of the build
     * @param ID The Build ID
     * @param name The Build name
     * @param descript The Build Description
     * @param direc The Build Directory
     */
     void receiveBuild(QString ID,QString name,QString descript, QString direc);


     /**
      * \fn void updateBuildName(int buildID, QString newBuildName);
      * @brief A function to update the new build Name to the xml file
      * @param buildID The Build Description
      * @param newBuildName The Build Directory
      */
     void updateBuildName(int buildID, QString newBuildName);

     /**
      * \fn void updateBuildName(int buildID, QString newBuildName);
      * @brief Finds the index of a build inside buildUniqueID
      * @param buildID The Build Description
      */
     int findBuildIndex(int buildID);

    void updateBuildDescr(int buildID, QString newBuildDescr);
	 
    void updateBuildDir(int buildID, QString newBuildDir);

    /**
     * \fn void removeBuild(int buildID);
     * @brief Removes the build from the xml file
     * @param buildID the ID of the build to be removed from the xml
     */
    void removeBuild(int buildID);

private:
    /**
     * \fn void RemoveBuildToBeUpdated(int buildID);
     * @brief A function which removes the build to be updated in order to update all variables
     * @param buildID the ID of the build which will temporarily be removed
     */
    void RemoveBuildToBeUpdated(int buildID);
private:
    /**
     * @var buildUniqueID
     * @brief A QMap of Build Numbers for the XML
     */
    QMap<QString,QString> buildUniqueID;

    /**
     * @var buildName
     * @brief A QMap of Build Names for the XML
     */
    QMap<QString,QString> buildName;

    /**
     * @var buildDescription
     * @brief A QMap of Build Descriptions for the XML
     */
    QMap<QString,QString> buildDescription;

    /**
     * @var buildDirectory
     * @brief A QMap of Build Directories for the XML
     */
    QMap<QString,QString> buildDirectory;

   /**
     * @var theXMLWriter
     * @brief A QXmlStreamWriter instance
     */
    QXmlStreamWriter* theXMLWriter;
};

#endif // XMLWRITER_H
