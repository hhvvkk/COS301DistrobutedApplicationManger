/**
* @file mainwindow.h
* @brief This is the file containing the main form which the user is greeted with when executing the application
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSystemTrayIcon>
#include <QString>
#include <QMap>
#include <QMapIterator>
#include <QDebug>
#include <QMessageBox>
//#include "windows.h"

#include "sysInfo.h"
#include "management.h"
#include "viewbuilds.h"
#include "addbuildfortest.h"

namespace Ui {
class MainWindow;
}


/**
 * @class MainWindow
 * @brief The MainWindow class will display the main Graphical User Interface for this application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    /**
     * \fn void quitApplication();
     * @brief The signal that is emitted when clicking exit on the tray item
     */
signals:
    /**
     * @brief quitApplication the quit signal
     */
    void quitApplication();

    
public:
    /**
     * @brief MainWindow The constructor creating the Graphical User Interface
     * @param parent
     * @param app The link to applicaiton so that it may be closed manually
     */
    explicit MainWindow(QWidget *parent = 0);
    /**
      * @fn ~MainWindow();
      * @brief the destructor for the main window
      */
    ~MainWindow();


private slots:
    void signalConnected(bool connectionValue);

    /**
     * \fn void quitTheApplication();
     * @brief A function emitting a signal called quitApplication which will then end application
     */
    void quitTheApplication();

    /**
     * @fn void showOrHideTrayClick();
     * @brief showOrHideTray will fire if the button show or hide is clicked in the tray
     */
    void showOrHideTrayClick();


    /**
     * @fn void connectClick();
     * @brief connectClick The function called when the user clicks Connect
     */
    void connectClick();

    /**
     * @fn void disconnectClick();
     * @brief disconnectClick The function called when the user clicks Disconnect
     */    
    void disconnectClick();

    /**
     * @fn void on_pushButtonViewBuilds_clicked();
     * @brief on_pushButtonViewBuilds_clicked will be fired once the user clicks on view builds or when user click build
     */
    void on_pushButtonViewBuilds_clicked();

    /**
    * \fn void initiateAddBuild(Build b);
    * @brief initiateAddBuild will emit a signal to add the build
    */
    void initiateAddBuild(Build b);

    /**
     * @fn void on_pushButton_clicked();
     * @brief on_pushButton_clicked the click function of the button
     */
    void on_pushButton_clicked();

    /**
     * @fn void on_pushButton_@_clicked();
     * @brief on_pushButton_@_clicked the click function of the button
     */
    void on_pushButton_2_clicked();

private:
    /**
     * @fn void showConnectedWidgets();
     * @brief A function to display all the widgets for a connected state
     */
    void showConnectedWidgets();

    /**
     * @fn void showDisconnectedWidgets();
     * @brief A function to display all the widgets for a disconnected state
     */
    void showDisconnectedWidgets();

private:
    /**
     * @var ui;
     * @brief ui the MainWindow user interface
     */
    Ui::MainWindow *ui;

    /**
     * @var trayIcon
     * @brief The tray icon that will be displayed for the client application
     */
    QSystemTrayIcon *trayIcon;


    /**
     * @var management
     * @brief management the Management facade object that houses the logic behind the application
     */
    Management *management;

};

#endif // MAINWINDOW_H
