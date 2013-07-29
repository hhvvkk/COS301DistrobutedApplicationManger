/**
* @file mainwindow.h
* @brief This is the file containing the main form which the user is greeted with when executing the application
* @author 101Solutions
* @version 2.0
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include "management.h"
/**
 * @class MainWindow
 * @brief The MainWindow class will display the main Graphical User Interface for this application
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    /**
     * @brief MainWindow The constructor creating the Graphical User Interface
     * @param parent
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
      * \fn ~MainWindow();
      * @brief the destructor for the main window
      */
    ~MainWindow();

private slots:
    /**
     * @brief connectClick The function called when the user clicks Connect
     */
    void connectClick();
    /**
     * @brief disconnectClick The function called when the user clicks Disconnect
     */
    void disconnectClick();

private:
    Ui::MainWindow *ui;
    /**
     * @brief management the Management facade object that houses the logic behind the application
     */
    Management *management;

};

#endif // MAINWINDOW_H
