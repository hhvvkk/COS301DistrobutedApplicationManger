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

#include "management.h"
#include "viewbuilds.h"

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
     * @brief connectClick The function called when the user clicks Connect
     */
    void connectClick();

    /**
     * @brief disconnectClick The function called when the user clicks Disconnect
     */    
    void disconnectClick();

    /**
     * @fn void on_pushButtonViewBuilds_clicked();
     * @brief on_pushButtonViewBuilds_clicked will be fired once the user clicks on view builds or when user click build
     */
    void on_pushButtonViewBuilds_clicked();

private:
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
