/**
* @file mainform.h
* @brief This is the file containing the main form that a user is greeted with
*/

#ifndef MAINFORM_H
#define MAINFORM_H

#include <QFile>
#include <QFrame>
#include <QGroupBox>
#include <QLabel>
#include <QLayout>
#include <QMainWindow>
#include <QMap>
#include <QMapIterator>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPicture>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QStringList>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QInputDialog>

#include "addsimulation.h"

#include "mainbuildinfo.h"
#include "abouthelp.h"
#include "aboutversion.h"
#include "addbuild.h"
#include "copybuildover.h"
#include "Build.h"
#include "inputdialogue.h"
#include "management.h"
#include "myDirIterator.h"
#include "server.h"
#include "xmlReader.h"
#include "moreinfo.h"
#include "simulation.h"
#include "watcher.h"


namespace Ui {
class MainForm;
}
/**
 * @class MainForm
 * @brief The MainForm class will be the main window that the user is greeted with when he opens the application
 */
class MainForm : public QMainWindow
{
    Q_OBJECT


signals:
    /**
     * \fn void quitApplication();
     * @brief The signal that is emitted when clicking exit on the tray item
     */
    void quitApplication();

public:
    /**
     * \fn MainForm(QWidget *parent = 0);
     * @brief MainForm This will construct the graphical user interface of the AppMan application
     * @param parent The parent that will be set for application
     * @param app The link to applicaiton so that it may be closed manually
     */
    explicit MainForm(QWidget *parent = 0);

    /**
      * \fn ~MainForm();
      * @brief A destructor for the main form
      */
    ~MainForm();

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
     * \fn virtual void dragMoveEvent(QDragMoveEvent *event);
     * @brief dragMoveEvent once the object is dragged and moved around this function is called
     * @param event the event that fired the dragMove
     */
    virtual void dragMoveEvent ( QDragMoveEvent *event );

private slots:

//    void getDetStats(QString stats);

  //  void getMinStats(QString stats);


    /**
     * \fn void quitTheApplication();
     * @brief A function emitting a signal called quitApplication which will then end application
     */
    void quitTheApplication();

    /**
     * \fn void showOrHideTrayClick();
     * @brief showOrHideTray is a function that will show the tray or hide the tray depending on its current state
     */
    void showOrHideTrayClick();

    /**
      * \fn void newSlaveConnected(Machine *m, int index);
      * @brief When a new slave connects this function is called
      */
    void newSlaveConnected(Machine *m);

    /**
     * \fn void initiateAddBuild(Build b);
     * @brief initiateAddBuild will emit a signal to add the build
     * @param b The build to be added
     */
    void initiateAddBuild(Build *b);


    void initiateAddSimulation(Simulation * sim);

    void showSimulations();


    /**
      * \fn void slaveDisconnected(int index);
      * @brief When a slave disconnects this function is called
      */
    void slaveDisconnected(int index);

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
     * @fn void masterBuildsClicked(QModelIndex index);
     * @brief The Build instance of the relevant instance is displayed in the buildInfo tree
     * @param index the index that is clicked on main form
     */
    void masterBuildsClicked(QModelIndex index);

    /**
     * @fn void slaveStatsClicked(QModelIndex index);
     * @brief Performed when the slave has been clicked
     * @param index the index that is clicked on main form
     */
    void slaveStatsClicked(QModelIndex index);

    /**
     * @fn void clearDockWidget();
     * @brief Clears the dock widget that will either contain build info or slave info
     */
    void clearDockWidget();
	
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

    /**
     * @fn void on_actionCopy_Build_Over_triggered();
     * @brief The function that opens a dialogue for a build to copy over
     */
    void on_actionCopy_Build_Over_triggered();

    /**
     * @fn void initiateCopyBuildOver(int uniqueId, QString buildName);
     * @brief Function that initiates a copy over from master machine
     * @param uniqueId The unique Id of the machine to which it will copy
     * @param buildName the build name which it will copy it over to
     */
    void initiateCopyBuildOver(int uniqueId, QString buildName);

    /**
     * @fn void slaveGotBuild(Machine*m, int buildId, bool buildExist);
     * @brief A function that is invoked once a machine successfully acknowledges a new build is added to their side
     * @param m The machine which acknowledges the new build
     * @param buildId the build id that the machine received
     * @param slaveBuildName the name that is saved on the slave side
     * @param buildExist A parameter indicating whether or not a build exist on master
     */
    void slaveGotBuild(Machine*m, int buildId,  QString slaveBuildName, bool buildExist);

    /**
    * \fn void slaveBuildSizeSame(int buildId, int machineId, bool isTheSame);
    * @brief A function showing whether the build is the same on the master with the slave
    * @param isSame A boolean indicating if it is the same or not
    * @param buildID The ID of the build which isSame value is being set for
    * @param machineId The ID of the machine which invoked the function
    */
    void slaveBuildSizeSame(int buildId, int machineId, bool isTheSame);


    /**
      * QString getCompressPath();
      * @brief Gets the directory where the builds will be compressed in(i.e. where the zip files will be located)
      * @return Returns the build compress directory
      */
    QString getCompressPath();

    /**
     * \fn void slaveBuildSynched(int machineId, int buildId, double percentageSynched);
     * @brief Function which indicates how far the build is synchronised on the user interface
     * @param buildId The ID of the build is the same or not
     * @param slaveId The ID of the machine which has the build
     * @param percentageSynched The percentage that the build is synchronised
     */
    void slaveBuildSynched(int machineId, int buildId,double percentageSynched);
    
    void on_treeWidgetSlaves_clicked(const QModelIndex &index);

    /**
     * @fn void slaveUpdatedBuildName(int machineID, int buildID, QString updatedName);
     * @brief A function to update the build name on the gui where a machine has updated the name for a build
     * @param machineID The machine which acknowledges the updated build name
     * @param buildID the build id that the machine received
     * @param updatedName the name that is saved on the slave side
     */
    void slaveUpdatedBuildName(int machineID, int buildID, QString updatedName);


    /**
     * \fn void buildInfoDoubleClicked(QTreeWidgetItem* theDoubleClickedItem, int theColumn);
     * @brief A function executing each time an item inside the build info is double clicked
     * @param theDoubleClickedItem The item that was double clicked
     * @param theColumn The index of the column of the item double clicked
     */
    void buildInfoDoubleClicked(QTreeWidgetItem* theDoubleClickedItem, int theColumn);


    /**
     * \fn void buildInfoItemEditedChanged(QTreeWidgetItem*, int column);
     * @brief A function executing each time an item inside the build info is double clicked
     * @param theDoubleClickedItem The item that was double clicked
     * @param theColumn The index of the column of the item double clicked
     */
    void buildInfoItemEditedChanged(QTreeWidgetItem*, int column);


    void on_actionAdd_Simulation_triggered();



    /**
     * \fn void buildActiveUse();
     * @brief A function resetting the timer which prolongues the collapse untill inactive
     */
    void buildActiveUse();

    /**
     * \fn void collapseMasterBuilds();
     * @brief A function executed after the masterBuilds menu must be collapsed(which happens when the timer runs out and inactive)
     */
    void collapseMasterBuilds();


    /**
     * \fn void slaveDeletedBuild(int machineID, int buildID);
     * @brief Slot connected such that each time a slave deletes a build and all its files, this function is called
     * @brief machineID The machine's ID of the machine who deleted the build sucessfully
     * @param buildID The build ID of the build that was deleted on the slave machine
     */
    void slaveDeletedBuild(int machineID, int buildID);


    /**
      * \fn void buildDeleted();
      * @brief Executed when the build has been deleted on the master machine and all machines are notified of the delete
      */
    void buildDeleted();

    /**
      * \fn void buildDeleted();
      * @brief The function fired when the user clicks on the infobox
      */
    void infoboxClicked(QModelIndex index);

    /**
      * \fn void mustResynch(int buildID);
      * @brief Slot connected such that each time a build has changed, after a amount of time, it will be resynched
      * @param buildID The ID to resynchronise on all machines
      */
    void mustResynch(int buildID);

    /**
      * \fn void MainForm::buildTimerCount(int buildID, int timeRemaining);
      * @brief Slot connected such that each time the timer counts down for the resyncrhonisation this function is called
      * @param buildID The ID to resynchronise on all machines
      * @param timeRemaining The amount of time remaining on the timer before the build will be resynchronised
      */
    void buildTimerCount(int buildID, int timeRemaining);
private:

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
     * @fn void loadXMLBuilds();
     * @brief Displays all builds in the xml
     */
    void loadXMLBuilds();
    /**
     * @fn void populateBuildInfo(Build *retr);
     * @brief Displays the selected slave in the xml
     * @param retr the build that was retrieved
     */
    void populateBuildInfo(Build *retr);
    /**
     * @fn void populateTreeWidgetInfo(Build *retr);
     * @brief Displays the selected slave in the xml
     * @param retr the build that was retrieved
     */
    void populateTreeWidgetInfo(Build *retr);

    /**
     * \fn QTreeWidgetItem* getSlaveTreeItemById(int uniqueId);
     * @brief Finds and returns the QTreeWidgetItem of the machine which has an IP adddress
     * @param uniqueId The unique Id of the machine
     * @return Returns the QTreeWidgetItem for a machine
     */
    QTreeWidgetItem* getSlaveTreeItemById(int uniqueId);

    /**
     * \fn void setBuildInfo(int setWhat, QString value)
     * @brief A function to set the piece of build information referred to by setWhat to the value passed through in management and the gui(in the case of name or number - directory is set elsewhere)
     * @param setWhat The value of the BuildInformationEnum which will be set
     * @param value The value to which it will be set
     * @param buildID The id of the build to set
     */
    void setBuildInfo(int setWhat, QString value, int buildID);

    /**
     * \fn bool mouseCurserOver(QWidget *theItem);
     * @brief A function which determines whether the item sent through is underneath the cursor
     * @param theItem The item which will be tested
     * @return Returns true if the mouse is over the item otherwise returns false
     */
    bool mouseCurserOver(QWidget *theItem);

    /**
     * \fn bool mouseCurserOver(QWidget *theItem);
     * @brief Displays the slavebuild info in the infobox and gets it by using the ID of the build
     * @param slaveBuildItem The item which was clicked
     * @param machineID The ID of the machine in which its build was clicked
     */
    void displaySlaveBuildInfo(QTreeWidgetItem *slaveBuildItem, int machineID);

    /**
     * \fn void slaveItemClicked(const QModelIndex &index);
     * @brief A function to be called when a slave item was called
     * @param index The index that was clicked
     */
    void slaveItemClicked(const QModelIndex &index);
private:
    /**
     * @var ui
     * @brief ui It is the graphical user interface
     */
    Ui::MainForm *ui;

    int NAME_SIZE_LIMIT;

    int DESCRIPTION_SIZE_LIMIT;

    /**
     * @var trayIcon
     * @brief trayIcon is a variable to create the tray icon for the application
     */
    QSystemTrayIcon *trayIcon;

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
    };
	
    /**
     * @var masterBuilds
     * @brief The pointer to a Masterbuilds object
     */
    MasterBuilds *masterBuilds;

    /**
     * @var buildInfo
     * @brief The pointer to a BuildInfo object
     */
    BuildInfo *buildInfo;

    /**
     * @class SlaveStats
     * @brief The SlaveStats class will be used to display information related to selected slaves
     */
    class SlaveStats: public QTreeWidget{
    public:
        SlaveStats(QWidget *parent = 0, int id = 0, QString input = "");
    };

    /**
     * @var slaveStats
     * @brief The pointer to a SlaveStats object
     */
    SlaveStats *slaveStats;


    enum BuildInformationEnum { BUILDDIRECTORY, BUILDNAME, BUILDNUMBER, BUILDDESCRIPTION };

    QTimer *collapseTimer;

    Watcher *watcher;

};


#endif // MAINFORM_H
