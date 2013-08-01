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
    ///connect slots for backward signalling
    connect(management, SIGNAL(newSlaveConnected()),this,SLOT(newSlaveConnected()));
    connect(management, SIGNAL(slaveDisconnected()),this,SLOT(slaveDisconnected()));

    masterBuilds = new MasterBuilds();
    masterBuilds->setHeaderHidden(true);


    /*
     * This part initializes the masterBuilds and adds it to mainform
     */
    QVBoxLayout *buildLayout = new QVBoxLayout();
    ui->groupBoxMaster->setLayout(buildLayout);
    buildLayout->addWidget(masterBuilds);


    QTreeWidgetItem *boola = new QTreeWidgetItem();
    boola->setText(0,"buildNo1");

        //boola->set
        masterBuilds->addTopLevelItem(boola);
        boola = new QTreeWidgetItem();
        boola->setText(0,"buildNo2");
        masterBuilds->addTopLevelItem(boola);boola = new QTreeWidgetItem();
        boola->setText(0,"buildNo3");
        masterBuilds->addTopLevelItem(boola);boola = new QTreeWidgetItem();
        boola->setText(0,"buildNo4");
        masterBuilds->addTopLevelItem(boola);

}


MainForm::~MainForm()
{
    delete ui;
    //this must be set to delete later or segmentation fault will occur!
    management->deleteLater();
}


MainForm::MasterBuilds::MasterBuilds(QWidget *parent)
    :QTreeWidget(parent){

}

void MainForm::MasterBuilds::mousePressEvent(QMouseEvent *event){

    /*Also not yet pointing to the qwidgetItem*/
    /*
    QRect widgetRect = this->geometry();
    QPoint mousePos = this->mapFromGlobal(QCursor::pos());
    */

    //the point where the event was fired
    QPoint hotSpot = event->pos();// - child->pos();

    QTreeWidget::mousePressEvent(event);

    QTreeWidgetItem  *theItem =  itemAt(hotSpot);

    if(!theItem){
        return;//return if it fails to dynamic cast
    }

    QMimeData *mimeData = new QMimeData;

    //set the mime data for mouse drag
    QString indexOfItem = QString::number(this->indexOfTopLevelItem(itemAt(hotSpot)));
    mimeData->setText("#<<MBIndex="+indexOfItem );//+ theItem->text(0));
    mimeData->setData("application/x-hotspot", QByteArray::number(hotSpot.x()) + " " + QByteArray::number(hotSpot.y()));


    qDebug()<<mimeData->text();

    QLabel *renderer = new QLabel();
    renderer->setText(theItem->text(0));

    QPixmap pixmap(renderer->size());
    renderer->render(&pixmap); //render the image of the item that will be dragged


    //create a QDrag file to set all the mimedata for drag and drop
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(hotSpot);

    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);


    if(dropAction == Qt::MoveAction){
        renderer->close();
        //Very very important! ")
        renderer->deleteLater();
    }
    else{
        renderer->close();
        //Very very important! ")
        renderer->deleteLater();
    }
}


void MainForm::dropEvent ( QDropEvent *event ){
    QTreeWidgetItem *slaveTreeWidget = dynamic_cast<QTreeWidgetItem*>(childAt(event->pos()));

    if (slaveTreeWidget){

        QString buildIndex = event->mimeData()->text();
        qDebug()<<"ASDASDADASDFASGAC=="+buildIndex;
        //check to see if it is the right item that is dropped
        if(!buildIndex.contains("#<<MBIndex="))
            return;
        dropBuildToSlave(buildIndex, "somethingForNow");
        event->accept();
        return;
    }
    else{
        qDebug()<<"...FALSE...";
    }
/*
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
*/
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
    //Machine* machines= management->getAllMachines();
    qDebug()<<"ASDASDS";
    ui->treeWidgetSlaves->clear();
    for(int i = 0; i <management->getMachineCount(); i++){
        QTreeWidgetItem *slaveItem = new QTreeWidgetItem();
        slaveItem->setText(0, management->getMachineAt(i)->getMachineIP());
        ui->treeWidgetSlaves->addTopLevelItem(slaveItem);
    }
}
