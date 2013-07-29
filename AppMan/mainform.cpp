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
    //connect(management, SIGNAL(newSlaveConnected()), this, SLOT(newSlaveConnected()));
    connect(management, SIGNAL(newSlaveConnected()),this,SLOT(newSlaveConnected()));
    /************************Master(setupBegin)***********************/
    QVBoxLayout *masterHorizontalLayout = new QVBoxLayout;

    spinBoxMaster = new QSpinBox();
    spinBoxMaster->setMinimum(0);
    masterHorizontalLayout->addWidget(spinBoxMaster);

    //create Horizontal Layout
    Build *masterBuild0 = new Build("masterBuild0",true);
    Build *masterBuild1 = new Build("masterBuild1",true);
    Build *masterBuild2 = new Build("masterBuild2",true);
    Build *masterBuild3 = new Build("masterBuild3",true);
    masterHorizontalLayout->addWidget(masterBuild0);
    masterHorizontalLayout->addWidget(masterBuild1);
    masterHorizontalLayout->addWidget(masterBuild2);
    masterHorizontalLayout->addWidget(masterBuild3);

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
     Build *slave0Build0 = new Build("slave0Build0",false);
     Build *slave0Build1 = new Build("slave0Build1",false);
     Build *slave0Build2 = new Build("slave0Build2",false);
     slave0HorizontalLayout->addWidget(slave0Build0);
     slave0HorizontalLayout->addWidget(slave0Build1);
     slave0HorizontalLayout->addWidget(slave0Build2);
     groupBoxSlave0 = new QGroupBox();
     groupBoxSlave0->setTitle("Slave0");
     groupBoxSlave0->setLayout(slave0HorizontalLayout);

     //slave1
     QVBoxLayout *slave1HorizontalLayout = new QVBoxLayout;
     Build *slave1Build0 = new Build("slave1Build0",false);
     Build *slave1Build1 = new Build("slave1Build1",false);
     Build *slave1Build2 = new Build("slave1Build2",false);
     slave1HorizontalLayout->addWidget(slave1Build0);
     slave1HorizontalLayout->addWidget(slave1Build1);
     slave1HorizontalLayout->addWidget(slave1Build2);
     groupBoxSlave1 = new QGroupBox();
     groupBoxSlave1->setTitle("Slave1");
     groupBoxSlave1->setLayout(slave1HorizontalLayout);


     //slave2
  /*   QVBoxLayout *slave2HorizontalLayout = new QVBoxLayout;
     Build *slave2Build0 = new Build("slave2Build0",false);
     Build *slave2Build1 = new Build("slave2Build1",false);
     Build *slave2Build2 = new Build("slave2Build2",false);
     slave2HorizontalLayout->addWidget(slave2Build0);
     slave2HorizontalLayout->addWidget(slave2Build1);
     slave2HorizontalLayout->addWidget(slave2Build2);
     groupBoxSlave2 = new QGroupBox();
     groupBoxSlave2->setTitle("Slave2");
     groupBoxSlave2->setLayout(slave2HorizontalLayout);
*/

     //spinboxSlaves
     spinBoxSlaves = new QSpinBox();
     slavesHorizontalLayout->addWidget(spinBoxSlaves);
     spinBoxSlaves->setMinimum(0);

     //add slaves to the main layout
     slavesHorizontalLayout->addWidget(groupBoxSlave0);
     slavesHorizontalLayout->addWidget(groupBoxSlave1);
    // slavesHorizontalLayout->addWidget(groupBoxSlave2);
     ui->groupBoxSlaves->setLayout(slavesHorizontalLayout);

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

void MainForm::newSlaveConnected(){
    //now go find slave and show it if needed
    displaySlaves();
}

void MainForm::displaySlaves(){
    int index = spinBoxSlaves->value();
    int machineCount = management->getMachineCount();

    qDebug()<<"NEW SLAVE CONNECTED..index"<<index;
    qDebug()<<"machineCount"<<machineCount;
    //get the machines that should be shown at index 1 and index2
    int show1 = index*2;
    int show2 = show1+1;

    Machine* m0 = management->getMachineAt(show1);
    Machine* m1 = management->getMachineAt(show2);

    //then finally display the machines
    if(m0 == 0){
        qDebug()<<"isZero";
        groupBoxSlave0->hide();
    }
    else{
        groupBoxSlave0->setTitle(m0->getMachineIP());
    }

    if(m1 == 0){
        groupBoxSlave1->hide();
    }else{
        groupBoxSlave1->setTitle(m1->getMachineIP());
    }


}
