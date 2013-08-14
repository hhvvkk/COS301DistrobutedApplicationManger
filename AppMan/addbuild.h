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
     * @fn void initiateAddBuild(Build b);
	 * @param b the Build to add
     * @brief initiateAddBuild will emit a signal to add the build
     */
    void initiateAddBuild(Build b);
public:
	/**
     * @fn AddBuild(Management *man, QString directory = "", QWidget *parent = 0);
     * @brief AddBuild This will construct the graphical user interface of the AppMan application AddBuild fuctionality
     * @param parent The parent that will be set for the application
	 * @param directory The directory that will be set for the application
	 * @param man The Management link that will be set for the application
     */
    explicit AddBuild(Management *man, QString directory = "", QWidget *parent = 0);
	/**
	 * @fn ~AddBuild();
	 * @brief The destructor
	 */
    ~AddBuild();
public:
	/**
     * @fn void addToXML(int num,QString name,QString descript,QString direc);
	 * @param num the Build number to add
	 * @param name the Build name to add
	 * @param descript the Build description to add
	 * @param direc the Build directory to add
     * @brief addToXML will add the current build to the xml file
     */
    void addToXML(int num,QString name,QString descript,QString direc);
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
     * @brief checkBuildNo looks if the buildnumber is already in the xml
     * @param buildNum the current build number being passed
     * @return a boolean indication wether the build number is taken
     */
    bool checkBuildNo(QString buildNum);

private:
	/**
	 * @var ui
	 * @brief the user interface to add a build
	 */
    Ui::AddBuild *ui;
	/**
	 * @var management
	 * @brief the managemennt link...
	 */
    Management *management;
	/**
	 * @var theXMLWriter
	 * @brief the xmlWriter object
	 */
    xmlWriter *theXMLWriter;
};

#endif // ADDBUILD_H
