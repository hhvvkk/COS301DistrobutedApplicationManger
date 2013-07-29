#include <mainform.h>
#include <ui_mainform.h>
#include <abouthelp.h>
#include <aboutversion.h>
#include "server.h"


MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    management = new Management();

    Server *server = new Server(management);
    management->setServer(server);

    server = 0;

    this->setAcceptDrops(true);

    //connect up sinals(newSlaveConnected, slaveDisconnected)...
    connect(management, SIGNAL(newSlaveConnected()),this,SLOT(newSlaveConnected()));
    connect(management, SIGNAL(slaveDisconnected()),this,SLOT(slaveDisconnected()));


    /************************Master(setupBegin)***********************/
    QVBoxLayout *masterHorizontalLayout = new QVBoxLayout;

    spinBoxMaster = new QSpinBox();
    spinBoxMaster->setMinimum(0);
    masterHorizontalLayout->addWidget(spinBoxMaster);

    //create Horizontal Layout
    BuildWidget *masterBuildWidget0 = new BuildWidget("masterBuildWidget0",true);
    BuildWidget *masterBuildWidget1 = new BuildWidget("masterBuildWidget1",true);
    BuildWidget *masterBuildWidget2 = new BuildWidget("masterBuildWidget2",true);
    BuildWidget *masterBuildWidget3 = new BuildWidget("masterBuildWidget3",true);
    masterHorizontalLayout->addWidget(masterBuildWidget0);
    masterHorizontalLayout->addWidget(masterBuildWidget1);
    masterHorizontalLayout->addWidget(masterBuildWidget2);
    masterHorizontalLayout->addWidget(masterBuildWidget3);

    //w = 250 h = 80 / scaledContents = true    --- frameShape = panel + frameShadow = sunken + width = 2
    QFrame *addFrame = new QFrame();
    addLabel = new QLabel();
    //addLabel->setFrameShadow();
    addLabel->setMaximumSize(250,80);
    addLabel->setScaledContents(true);

    if(QFile("./images/Directory.png").exists() == false)
        addLabel->setText("+ Add");
    else{
        QPixmap pixMap = QPixmap("./images/Add.png");
        addLabel->setPixmap(pixMap);
        addLabel->setText("");
    }
    addLabel->setFrameShadow(QFrame::Raised);
    addLabel->setFrameShape(QFrame::Box);
    addLabel->setLineWidth(2);
    addLabel->setAcceptDrops(true);

    QVBoxLayout *boxLayout = new QVBoxLayout;
    boxLayout->addWidget(addLabel);
    addFrame->setLayout(boxLayout);

    addFrame->setAcceptDrops(true);
    masterHorizontalLayout->addWidget(addFrame);
    ui->groupBoxMaster->setLayout(masterHorizontalLayout);
    /************************Master(setupEnd)***********************/


    /************************Slave(setupBegin)***********************/
     QVBoxLayout *slavesHorizontalLayout = new QVBoxLayout;

     //slave0

     QVBoxLayout *slave0HorizontalLayout = new QVBoxLayout;
     offlineDisplay0 = new QPushButton("offlineShow");
     slave0BuildWidget0 = new BuildWidget("slave0BuildWidget0",false);
     slave0BuildWidget1 = new BuildWidget("slave0BuildWidget1",false);
     slave0BuildWidget2 = new BuildWidget("slave0BuildWidget2",false);
     slave0HorizontalLayout->addWidget(offlineDisplay0);
     slave0HorizontalLayout->addWidget(slave0BuildWidget0);
     slave0HorizontalLayout->addWidget(slave0BuildWidget1);
     slave0HorizontalLayout->addWidget(slave0BuildWidget2);
     groupBoxSlave0 = new QGroupBox();
     groupBoxSlave0->setTitle("Slave0");
     groupBoxSlave0->setLayout(slave0HorizontalLayout);

     //slave1
     QVBoxLayout *slave1HorizontalLayout = new QVBoxLayout;
     offlineDisplay1 = new QPushButton("offlineShow");
     slave1BuildWidget0 = new BuildWidget("slave1BuildWidget0",false);
     slave1BuildWidget1 = new BuildWidget("slave1BuildWidget1",false);
     slave1BuildWidget2 = new BuildWidget("slave1BuildWidget2",false);
     slave1HorizontalLayout->addWidget(offlineDisplay1);
     slave1HorizontalLayout->addWidget(slave1BuildWidget0);
     slave1HorizontalLayout->addWidget(slave1BuildWidget1);
     slave1HorizontalLayout->addWidget(slave1BuildWidget2);
     groupBoxSlave1 = new QGroupBox();
     groupBoxSlave1->setTitle("Slave1");
     groupBoxSlave1->setLayout(slave1HorizontalLayout);


     //icons for offline buttons
     if(QFile("./images/Offline.png").exists() == false){
         offlineDisplay1->setText("OFFLINE");
         offlineDisplay0->setText("OFFLINE");
     }
     else{
         offlineDisplay1->setIcon(QIcon("./images/Offline.png"));
         offlineDisplay1->setText("");
         offlineDisplay0->setIcon(QIcon("./images/Offline.png"));
         offlineDisplay0->setText("");
     }
     //seticonsizes
     const static QSize defaultSize = QSize(200,50);
     offlineDisplay0->setIconSize(defaultSize);
     offlineDisplay1->setIconSize(defaultSize);

     //spinboxSlaves
     spinBoxSlaves = new QSpinBox();
     slavesHorizontalLayout->addWidget(spinBoxSlaves);
     spinBoxSlaves->setMinimum(0);

     //add slaves to the main layout
     slavesHorizontalLayout->addWidget(groupBoxSlave0);
     slavesHorizontalLayout->addWidget(groupBoxSlave1);
     ui->groupBoxSlaves->setLayout(slavesHorizontalLayout);

     displaySlaves();
     connect(spinBoxSlaves, SIGNAL(valueChanged(int)), this,SLOT(spinboxChanged()));
    /************************Slave(setupEnd)***********************/
}

MainForm::~MainForm()
{
    delete ui;
    //this must be set to delete later or segmentation fault will occur!
    management->deleteLater();
}


void MainForm::dropEvent ( QDropEvent *event ){
    QGroupBox *grpBox = dynamic_cast<QGroupBox*>(childAt(event->pos()));
    if (grpBox){//THIS MEANS THERE IS A DROPPED ITEM OVER A group box(E>G> Slave)
        //i.e. it could be a build that is dropped for a slave
        //here it checks if it is dropped on a slave group box or not
        if(!(groupBoxSlave0 == grpBox))
            if(!(groupBoxSlave1 == grpBox))
                //if(!(groupBoxSlave2 == grpBox))
                    return;

        QString fromBuild = event->mimeData()->text();

        //check to see if it is the right label that is dropped
        if(!fromBuild.contains("dragmasterBuild"))
            return;
        dropBuildToSlave(fromBuild, grpBox->title());
        event->accept();
        return;
    }

    QLabel *labelTrue = dynamic_cast<QLabel*>(childAt(event->pos()));
    if(labelTrue){//droppped on a label
        //check to see if it is the addLabel
        if(!(addLabel == labelTrue))
            return;

        QList<QUrl> list = event->mimeData()->urls();

        if(list.size() == 0)
            return;
        QString urlFromMime = list.at(0).toString();

        //this accept lets the other whatsits name know that the event have been accepted
        dropNewBuildAdd(urlFromMime);
        event->accept();
        return;
    }

}

void MainForm::dropBuildToSlave(QString fromBuild, QString toSlave){
    QMessageBox msgBox;
    msgBox.setText(fromBuild+"<-- slaveDropped -->"+toSlave);
    msgBox.exec();
}

void MainForm::dropNewBuildAdd(QString newBuildDirectory){
    QMessageBox msgBox;
    msgBox.setText("Label: "+newBuildDirectory);
    msgBox.exec();
}

void MainForm::dragMoveEvent ( QDragMoveEvent *event ){
    event->accept();
}


void MainForm::dragEnterEvent ( QDragEnterEvent *event ){
    event->accept();
}

void MainForm::dragLeaveEvent ( QDragLeaveEvent *event ){
    event->accept();
}

void MainForm::on_actionHelp_triggered(){
    AboutHelp *abHelp = new AboutHelp();
    abHelp->show();
}

void MainForm::on_actionVersion_triggered(){
    AboutVersion *abversion = new AboutVersion();
    abversion->show();
}

void MainForm::on_actionStart_triggered()
{
    //start the server
    management->startServer();
}

void MainForm::on_actionSet_Port_triggered()
{
    //set server port
    InputDialogue *inputDialogue = new InputDialogue("port", management);
    inputDialogue->show();
}

void MainForm::on_actionStop_Server_triggered()
{
    //stop the server
    management->stopServer();
}

void MainForm::spinboxChanged(){
    displaySlaves();
}

void MainForm::newSlaveConnected(){
    //now go find slave and show it if needed
    displaySlaves();
}

void MainForm::slaveDisconnected(){
    //now go find slave and show it if needed
    //it will find what slave has been disconnected
    qDebug()<<"slavedc's";
    qDebug()<<"calling displaySlaves()";
    displaySlaves();
}

void MainForm::displaySlaves(){
    int index = spinBoxSlaves->value();
    int machineCount = management->getMachineCount();
    int max=0;
    if(machineCount%2 == 0)
        max = (machineCount/2)-1;
    else
        max = (machineCount/2);
    if(max < 0)
        spinBoxSlaves->setMaximum(0);
    else
        spinBoxSlaves->setMaximum(max);;
    qDebug()<<"machineCount"<<machineCount;
    //get the machines that should be shown at index 1 and index2
    int show1 = index*2;
    int show2 = show1+1;

    Machine* m0 = management->getMachineAt(show1);
    Machine* m1 = management->getMachineAt(show2);

    //then finally display the machines
    if(m0 == 0){
        groupBoxSlave0->hide();
    }
    else{
        groupBoxSlave0->show();
        groupBoxSlave0->setTitle(m0->getMachineIP());

        slave0BuildWidget0->show();
        slave0BuildWidget1->show();
        slave0BuildWidget2->show();
        offlineDisplay0->hide();
        if(!m0->getMachineStatus().compare("offline")){
            //means the machine has gone offline and offline button needs to be shown
            slave0BuildWidget0->hide();
            slave0BuildWidget1->hide();
            slave0BuildWidget2->hide();
            offlineDisplay0->show();
        }
    }

    if(m1 == 0){
        groupBoxSlave1->hide();
    }else{
        groupBoxSlave1->show();
        groupBoxSlave1->setTitle(m1->getMachineIP());
        slave1BuildWidget0->show();
        slave1BuildWidget1->show();
        slave1BuildWidget2->show();
        offlineDisplay1->hide();
        if(!m1->getMachineStatus().compare("offline")){
            //means the machine has gone offline and offline button needs to be shown
            slave1BuildWidget0->hide();
            slave1BuildWidget1->hide();
            slave1BuildWidget2->hide();
            offlineDisplay1->show();
        }
    }


}
