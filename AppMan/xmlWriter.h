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
	 * @fn CreateXMLFile();
	 * @brief The function to create and append to the XML file
	 */
    void CreateXMLFile();
	/**
	 * @fn void receiveBuild(QString num,QString name,QString descript, QString direc);
	 * @brief The function that accepts parameters of the build
	 * @param num The Build number
	 * @param name The Build name
	 * @param descript The Build Description
	 * @param direc The Build Directory
	 */
    void receiveBuild(QString num,QString name,QString descript, QString direc);

private:
	/**
	 * @var buildNumber
	 * @brief A QMap of Build Numbers for the XML
	 */
    QMap<QString,QString> buildNumber;
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
