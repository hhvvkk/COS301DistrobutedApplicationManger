/**
* @file mainform.h
* @brief This is the file containing the main form that a user is greeted with
* @author 101Solutions
* @version 2.0
*/

#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QString>
#include <QWidget>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QHBoxLayout>
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

#include "management.h"
#include "inputdialogue.h"

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
    class BuildWidget : public QGroupBox{
    public:
        /**
         * \fn BuildWidget(QString ObjName, bool isMaster) : QGroupBox(ObjName){
         * @brief  BuildWidget This instantiates the class for either the master or the slave to display bulids
         * @param ObjName This is the name of the object such as "master BuildWidget3" or "slave BuildWidget1"
         * @param isMaster This boolean indicates the behaviour of the class to be master or a slave
         */
         BuildWidget(QString ObjName, bool isMaster) : QGroupBox(ObjName){
            QPushButton *newName = new QPushButton("button"+ObjName);
            QPushButton *newDirectory = new QPushButton("directory"+ObjName);
            QPushButton *newDescription = new QPushButton("description"+ObjName);
            QLabel *newDragLabel = new QLabel("drag");
            newDragLabel->setObjectName("drag"+ObjName);
            QPushButton *newRunButton = new QPushButton("runButton"+ObjName);
            QProgressBar *newProgressBar = new QProgressBar();

            newProgressBar->setValue(60);
            newProgressBar->setAlignment(Qt::AlignCenter);
            /*set the sizes and icons*/
            //set icons
            if(QFile("./images/Files.png").exists() == false)
                newName->setText("[] Name");
            else
                newName->setIcon(QIcon("./images/Files.png"));
            newName->setMaximumWidth(80);



            if(QFile("./images/Directory.png").exists() == false)
                newDirectory->setText("Directory");
            else{
                newDirectory->setIcon(QIcon("./images/Directory.png"));
                newDirectory->setText("");
            }

            if(QFile("./images/DescriptionGreen.png").exists() == false)
                newDescription->setText("Description");
            else{
                newDescription->setIcon(QIcon("./images/DescriptionGreen.png"));
                newDescription->setText("");
            }

            if(QFile("./images/Directory.png").exists() == false)
                newRunButton->setText("|> Run");
            else{
                newRunButton->setIcon(QIcon("./images/Run.png"));
                newRunButton->setText("");
            }


            //set icon sizes
            const static QSize newNameIconSize = QSize(20,30);
            const static QSize defaultSize = QSize(100,30);
            newName->setIconSize(newNameIconSize);
            newDirectory->setIconSize(defaultSize);
            newDescription->setIconSize(defaultSize);
            newRunButton->setIconSize(defaultSize);

            //add them to  BuildWidget
            QHBoxLayout *newHorizontalLayout = new QHBoxLayout;
            newHorizontalLayout->addWidget(newName);
            if(isMaster){
                newHorizontalLayout->addWidget(newDirectory);
                delete newRunButton;
            }
            else{
                newHorizontalLayout->addWidget(newRunButton);
                delete newDirectory;
            }

            if(isMaster)
                newHorizontalLayout->addWidget(newDescription);
            else{
                newHorizontalLayout->addWidget(newProgressBar);
            }


            if(isMaster)
                newHorizontalLayout->addWidget(newDragLabel);
            else
                delete newDragLabel;

            this->setLayout(newHorizontalLayout);
        }

    protected:
        /**
         * \fn void mousePressEvent(QMouseEvent *event){
         * @brief mousePressEvent this event is overridden to set the mimedata for later use
         * @param event the event fired when clicking on a label to drag and drop it
         */
        void mousePressEvent(QMouseEvent *event){
            //static cast an element that is pressed to QLabel
            QLabel *child = dynamic_cast<QLabel*>(childAt(event->pos()));

            if(!child){
                return;//return if it fails to dynamic cast
            }

            //the position where it will be created
            QPoint hotSpot = event->pos() - child->pos();

            //Set the mime data for the mouse drag (QDrag)
            QMimeData *mimeData = new QMimeData;
            mimeData->setText(child->objectName());
            mimeData->setData("application/x-hotspot", QByteArray::number(hotSpot.x()) + " " + QByteArray::number(hotSpot.y()));

            //set the size of the item that will be dragged and dropped
            QPixmap pixmap(child->size());
            child->render(&pixmap); //render the image of the item that will be dragged

            //create a QDrag file to set all the mimedata for drag and drop
            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(hotSpot);

            Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);

            if (dropAction == Qt::MoveAction)
                child->close();
        }

    };

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
      * \fn void newSlaveConnected();
      * @brief When a slave disconnects this function is called
      */
    void slaveDisconnected();

    /**
     * @brief on_actionHelp_triggered The function called when the About>Help function is called
     */
    void on_actionStop_Server_triggered();
    /**
     * @brief on_actionSet_Port_triggered The set port dropdown menu item clicked
     */
    void on_actionSet_Port_triggered();
    /**
     * @brief on_actionStart_triggered The start dropdown menu item clicked
     */
    void on_actionStart_triggered();
    /**
     * @brief on_actionHelp_triggered The Help menu dropdown item clicked
     */
    void on_actionHelp_triggered();

    /**
     * @brief on_actionVersion_triggered The function called when the About>Version function is called
     */
    void on_actionVersion_triggered();

    /**
     * \fn spinboxChanged();
     * @brief spinboxChanged The function that will be called once the spinbox changes values
     */
    void spinboxChanged();

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
     * @var spinBoxSlaves
     * @brief spinBoxSlaves The slave computer traversing mechanisim
     * @var spinBoxMaster
     * @brief spinBoxMaster The master build traversing mechanism
     */
    QSpinBox *spinBoxSlaves;
    QSpinBox *spinBoxMaster;

    /**
     * @var addLabel
     * @brief addLabel A label that will have a drop mode enabled to drop builds onto it
     */
    QLabel *addLabel;

    //Slave  BuildWidget displays
    /**
     * @var groupBoxSlave0
     * @brief groupBoxSlave0 The machine 0 at an index with regards to spinbox value
     */
    QGroupBox *groupBoxSlave0;
    /**
     * @var groupBoxSlave1
     * @brief groupBoxSlave1 The machine 1 at an index with regards to spinbox value
     */
    QGroupBox *groupBoxSlave1;

    /**
     * @brief offlineDisplay0 The machine 0 status when it is offline
     * @brief offlineDisplay1 The machine 1 status when it is offline
     */
    QPushButton *offlineDisplay0;
    QPushButton *offlineDisplay1;


    /**
     * @brief slave0BuildWidget0 Machine 0 build 0 displayed here
     * @brief slave0BuildWidget1 Machine 0 build 1 displayed here
     * @brief slave0BuildWidget2 Machine 0 build 2 displayed here
     */
    BuildWidget *slave0BuildWidget0;
    BuildWidget *slave0BuildWidget1;
    BuildWidget *slave0BuildWidget2;


    /**
     * @brief slave1BuildWidget0 Machine 1 build 0 displayed here
     * @brief slave1BuildWidget1 Machine 1 build 1 displayed here
     * @brief slave1BuildWidget2 Machine 1 build 2 displayed here
     */
    BuildWidget *slave1BuildWidget0;
    BuildWidget *slave1BuildWidget1;
    BuildWidget *slave1BuildWidget2;
};


#endif // MAINFORM_H
