/**
* @file mainform.h
* @brief This is the file containing the main form that a user is greeted with
*/

#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QGroupBox>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFile>
#include <QProgressBar>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QPicture>
#include <QFrame>
#include <QUrl>
#include <QSpinBox>
#include <QTreeWidgetItem>
#include <QMap>
#include <QMapIterator>
#include <QStringList>

#include "management.h"
#include "inputdialogue.h"
#include "Build.h"
#include "xmlReader.h"
#include "myDirIterator.h"

namespace Ui {
class MainForm;
}
/**
 * @class MainForm
 * @brief The MainForm class will be the class that the user is greeted with when he opens the application
 */
class MainForm : public QMainWindow
{
    Q_OBJECT
    
public:
    /**
     * \fn MainForm(QWidget *parent = 0);
     * @brief MainForm This will construct the graphical user interface of the AppMan application
     * @param parent The parent that will be set for application
     */
    explicit MainForm(QWidget *parent = 0);

    /**
      * \fn ~MainForm();
      * @brief A destructor for the main form
      */
    ~MainForm();
    
    //settings//

    //the  BuildWidget class to show the  BuildWidgets on the master and slaves
    /**
     * @class BuildWidget
     * @brief The BuildWidget class will be used to display a build for the user. It is a class that is created depending on whether it is a slave machine or not
     */


protected:
    /**
     * \fn virtual void dropEvent ( QDropEvent *event );
     * @brief dropEvent once the Build is dropped this is called
     * @param event the event that fired the drop
     */
    virtual void dropEvent ( QDropEvent *event );
    /**
     * \fn virtual void dragEnterEvent ( QDragEnterEvent *event );
     * @brief dragEnterEvent once the Build is dragged and it is in view
     * @param event the event that fired the dragEnter
     */
    virtual void dragEnterEvent ( QDragEnterEvent *event );
    /**
     * \fn virtual void dragLeaveEvent ( QDragLeaveEvent *event );
     * @brief dragLeaveEvent once the Build leaves the view
     * @param event the event that fired the dragLeave
     */
    virtual void dragLeaveEvent ( QDragLeaveEvent *event );
    /**
     * @brief dragMoveEvent once the object is dragged and moved around this function is called
     * @param event the event that fired the dragMove
     */
    virtual void dragMoveEvent ( QDragMoveEvent *event );
private slots:
    /**
      * \fn void newSlaveConnected();
      * @brief When a new slave connects this function is called
      */
    void newSlaveConnected();

    /**
     * \fn void initiateAddBuild(Build b);
     * @brief initiateAddBuild will emit a signal to add the build
     */
    void initiateAddBuild(Build b);

    /**
      * \fn void newSlaveConnected();
      * @brief When a slave disconnects this function is called
      */
    void slaveDisconnected();

    /**
	 * @fn void on_actionStop_Server_triggered();
     * @brief on_actionStop_Server_triggered The function called when the server is stopped
     */
    void on_actionStop_Server_triggered();
    /**
	 * @fn void on_actionSet_Port_triggered();
     * @brief on_actionSet_Port_triggered The set port dropdown menu item clicked
     */
    void on_actionSet_Port_triggered();
    /**
	 * @fn void on_actionStart_triggered();
     * @brief on_actionStart_triggered The start dropdown menu item clicked
     */
    void on_actionStart_triggered();
    /**
	 * @fn void on_actionHelp_triggered();
     * @brief on_actionHelp_triggered The Help menu dropdown item clicked
     */
    void on_actionHelp_triggered();

    /**
	 * @fn void on_actionVersion_triggered();
     * @brief on_actionVersion_triggered The function called when the About>Version function is called
     */
    void on_actionVersion_triggered();

    /**
     * \fn spinboxChanged();
     * @brief spinboxChanged The function that will be called once the spinbox changes values
     */
    void spinboxChanged();

    /**
	 * @fn void masterBuildsClicked(QModelIndex index);
     * @brief The Build instance of the relevant instance is displayed in the buildInfo tree
     * @param index
     */
    void masterBuildsClicked(QModelIndex index);
	
	/**
	 * @fn void on_actionAdd_Build_triggered();
	 * @brief on_actionAdd_Build_triggered The function that will be called to add a build
	 */
    void on_actionAdd_Build_triggered();

	/**
	 * @fn displayBuilds();
	 * @brief displayBuilds The function to show the builds on the main form
	 */
    void displayBuilds();

private:
    /**
     * \fn dropBuildToSlave(QString from, QString to);
     * @brief dropBuildToSlave this function will call the copy function to copy a build over from master to slave
     * @param from The object name of the item that is dragged
     * @param to The item on which it is dropped(the slave name)
     */
    void dropBuildToSlave(QString from, QString to);

    /**
     * @brief dropNewBuildAdd This function will add new builds on the master computer and save it accordingly
     * @param newBuildDirectory this is the place on hard drive where the build is located
     */
    void dropNewBuildAdd(QString newBuildDirectory);

    /**
     * \fn displaySlaves
     * @brief displaySlaves A function that will be called to display the slave computer. This is called each time a slave machine status changes and a new slave connects
     */
    void displaySlaves();

    /**
     * @fn void loadXMLslaves();
     * @brief Displays all slaves in the xml
     */
    void loadXMLslaves();
    /**
     * @fn void populateBuildInfo(Build retr);
     * @brief Displays the selected slave in the xml
     * @param retr the build that was retrieved
     */
    void populateBuildInfo(Build retr);
    /**
     * @fn void populateTreeWidgetInfo(Build retr);
     * @brief Displays the selected slave in the xml
     * @param retr the build that was retrieved
     */
    void populateTreeWidgetInfo(Build retr);

private:
    /**
     * @var ui
     * @brief ui It is the graphical user interface
     */
    Ui::MainForm *ui;

    /**
     * @var management
     * @brief management This management class is the object facade to manage the logic of the application
     */
    Management *management;

    /**
     * @class MasterBuilds
     * @brief The MasterBuilds class will be used to display builds on the master machine
     */
    class MasterBuilds: public QTreeWidget{
    public:
        MasterBuilds(QWidget *par = 0);

    protected:
        /**
         * \fn mousePressEvent(QMouseEvent *event);
         * @brief A custom mouse press event to enable drag and drop
         * @param event
         */
        void mousePressEvent(QMouseEvent *event);
    };
	
	/**
	 * @var masterBuilds
	 * @brief The pointer to a Masterbuilds object
	 */
    MasterBuilds *masterBuilds;

    /**
     * @class BuildInfo
     * @brief The BuildInfo class will be used to display information related to selected builds
     */
    class BuildInfo: public QTreeWidget{
    public:
        BuildInfo(QWidget *parent = 0);
    };

	/**
	 * @var buildInfo
	 * @brief The pointer to a BuildInfo object
	 */
    BuildInfo *buildInfo;
};


#endif // MAINFORM_H
