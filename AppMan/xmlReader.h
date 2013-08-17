#ifndef XMLREADER_H
#define XMLREADER_H

#include <QXmlStreamReader>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QFile>

class xmlReader{
public:
    /**
     * @fn xmlReader()
     * @brief The Constructor
     */
    xmlReader();

    /**
     * @fn ~xmlReader()
     * @brief The Destructor
     */
    ~xmlReader();

    /**
     * @fn void parseXML()
     * @brief looks to see if xml is well formed etc
     */
    void parseXML();

    /**
     * @fn QMap<QString,QString> getBuildNumber()
     * @brief returns a QMap of buildNumbers
     * @return buildNumber
     */
    QMap<QString,QString> getBuildNumber();

    /**
     * @fn QMap<QString,QString> getBuildName()
     * @brief returns a QMap of buildNames
     * @return buildName
     */
    QMap<QString,QString> getBuildName();

    /**
     * @fn QMap<QString,QString> getBuildDescription()
     * @brief returns a QMap of buildDescriptions
     * @return buildDescription
     */
    QMap<QString,QString> getBuildDescription();

    /**
     * @fn QMap<QString,QString> getBuildDirectory()
     * @brief returns a QMap of buildDirectorys
     * @return buildDirectory
     */
    QMap<QString,QString> getBuildDirectory();

private:

    /**
     * @fn void parseBuild(QXmlStreamReader& xml)
     * @brief parseBuild loops through xml document and gets values of builds
     * @param xml is a QXmlStreamReader instance of the specified file
     */
    void parseBuild(QXmlStreamReader& xml);

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
};

#endif // XMLREADER_H
