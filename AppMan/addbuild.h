/**
* @file addbuild.h
* @brief This is the file containing the addBuild form that a user uses to add a build
*/
#ifndef ADDBUILD_H
#define ADDBUILD_H

#include <QWidget>
#include "management.h"
#include "Build.h"
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QMessageBox>
#include "xmlWriter.h"
#include "xmlReader.h"
#include <QPixmap>
#include <QImage>
#include <QMapIterator>

namespace Ui {
class AddBuild;
}

/**
 * @class AddBuild
 * @brief The AddBuild class will be used to display a dialogue to add a new build to the AppMan program
 */
class AddBuild : public QWidget
{
    Q_OBJECT
    
signals:
    /**
     * @fn void initiateAddBuild(Build *b);
     * @param b the Build to add
     * @brief initiateAddBuild will emit a signal to add the build
     */
    void initiateAddBuild(Build *b);

public:
    /**
     * @fn AddBuild(Management *man, QString directory = "", QWidget *parent = 0);
     * @brief AddBuild This will construct the graphical user interface of the AppMan application AddBuild fuctionality
     * @param parent The parent that will be set for the application
     * @param directory The directory that will be set for the application
     */
    explicit AddBuild(QString directory = "", QWidget *parent = 0);

	/**
	 * @fn ~AddBuild();
	 * @brief The destructor
	 */
    ~AddBuild();

public:
    /**
     * @fn void addToXML(int ID,QString name,QString descript,QString direc);
     * @param ID the Build ID to add
     * @param name the Build name to add
     * @param descript the Build description to add
     * @param direc the Build directory to add
     * @brief addToXML will add the current build to the xml file
     */
    void addToXML(int ID,QString name,QString descript,QString direc);

private slots:
    /**
     * @fn void cancelClick();
     * @brief cancelClick will close the form
     */
    void cancelClick();

    /**
     * @fn void okClick();
     * @brief okClick will add the build to the program
     */
    void okClick();

    /**
     * @fn void showError(QString errorMessage);
     * @brief showError shows an error message
     * @param errorMessage the message to show
     * @param info The type of message
     */
    void showError(QString errorMessage, QString info);

    /**
     * @fn void chooseClick();
     * @brief chooseClick will open a dialogue to choose the directory of the build
     */
    void chooseClick();

    /**
     * @brief checkBuildID looks if the build ID is already in the xml
     * @param buildID the current build ID being passed
     * @return a boolean indication wether the build ID is taken
     */
    bool checkBuildID(QString buildID);

    /**
     * \fn bool checkBuildName(QString buildName);
     * @brief checkBuildName looks if the build name is already in the xml
     * @param buildName the current build name being passed
     * @return a boolean indication wether the build name is taken
     */
    bool checkBuildName(QString buildName);

private:
	/**
	 * @var ui
	 * @brief the user interface to add a build
	 */
    Ui::AddBuild *ui;
	/**
	 * @var theXMLWriter
	 * @brief the xmlWriter object
	 */
    xmlWriter *theXMLWriter;

    int NAME_SIZE_LIMIT;
    int DESCRIPTION_SIZE_LIMIT;
};

#endif // ADDBUILD_H
