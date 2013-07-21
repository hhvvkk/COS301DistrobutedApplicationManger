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

namespace Ui {
class MainForm;
}


class MainForm : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
    
private:
    Ui::MainForm *ui;

    QSpinBox *spinBoxSlaves;
    QSpinBox *spinBoxMaster;
    //droppable items(i.e. there are items that can be dropped on them...
    QLabel *addLabel;
    QGroupBox *groupBoxSlave0;
    QGroupBox *groupBoxSlave1;
    //QGroupBox *groupBoxSlave2;

    //settings//
    //int numSlavesShown = 2;

    //the build class to show the builds on the master and slaves
    class Build : public QGroupBox{
    public:
        /**
         * @brief Build This instantiates the class for either the master or the slave to display bulids
         * @param ObjName This is the name of the object such as "masterBuild3" or "slaveBuild1"
         * @param isMaster This boolean indicates the behaviour of the class to be master or a slave
         */
        Build(QString ObjName, bool isMaster) : QGroupBox(ObjName){
            QPushButton *newName = new QPushButton("aDir"+ObjName);
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

            //add them to build
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
    virtual void dropEvent ( QDropEvent *event );
    virtual void dragEnterEvent ( QDragEnterEvent *event );
    virtual void dragLeaveEvent ( QDragLeaveEvent *event );
    virtual void dragMoveEvent ( QDragMoveEvent *event );
private:
    /**
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
};


#endif // MAINFORM_H
