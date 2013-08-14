#include "mainform.h"
#include "ui_mainform.h"



MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    management = new Management();
    Server *server = new Server(management);
    management->setServer(server);
    ///connect slots for backward signalling
    connect(management, SIGNAL(newSlaveConnected(Machine*, int)),this,SLOT(newSlaveConnected(Machine*, int)));
    connect(management, SIGNAL(slaveDisconnected(Machine*, int)),this,SLOT(slaveDisconnected(Machine*, int)));

    masterBuilds = new MasterBuilds();
    masterBuilds->setHeaderHidden(true);


    /*
     * This part initializes the masterBuilds and adds it to mainform
     */
    QVBoxLayout *buildLayout = new QVBoxLayout();
    ui->groupBoxMaster->setLayout(buildLayout);
    buildLayout->addWidget(masterBuilds);

    /*
     *TOP DOCKWIDGET(BEGIN)
     **/
    QVBoxLayout *topDocWidgetLayout = new QVBoxLayout();
    buildInfo = new BuildInfo();

    topDocWidgetLayout->addWidget(buildInfo);
   // ui->dockWidgetProperty->layout()->addWidget(buildInfo);

    ui->dockWidgetContents->setLayout(topDocWidgetLayout);
    /*
     *TOP DOCKWIDGET(END)
     **/

    //ui->dockWidgetProperty->setLayout();
    connect(masterBuilds, SIGNAL(clicked(QModelIndex)), this, SLOT(masterBuildsClicked(QModelIndex)));
    loadXMLBuilds();

    management->startServer();

    /*
     *Create the tray(BEGIN)
     **/
    trayIcon = new QSystemTrayIcon(QIcon(":/images/images/ALogo.png"), this);
    QAction *quitAction = new QAction( "Exit", trayIcon );
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));
    QAction *hideAction = new QAction( "Show/Hide", trayIcon );
    connect(hideAction, SIGNAL(triggered()), this, SLOT(showOrHideTrayClick()));

    QMenu *trayMenu = new QMenu;
    trayMenu->addAction(hideAction);
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();
    //setWindowFlags(windowFlags() | Qt::Tool);
    /*
     *Create the tray(END)
     **/
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

MainForm::BuildInfo::BuildInfo(QWidget *parent)
    :QTreeWidget(parent){

    QString labelHeader1 = "Property";
    QString labelHeader2 = "Value";
    QStringList labelHeaders;
    labelHeaders << labelHeader1<<labelHeader2;

    QTreeWidget::setColumnCount(2);

    QTreeWidget::setHeaderLabels(labelHeaders);

    QTreeWidgetItem *newItem;

    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Build Number");
    this->addTopLevelItem(newItem);

    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Name");
    this->addTopLevelItem(newItem);


    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Directory");
    this->addTopLevelItem(newItem);


    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Description");
    this->addTopLevelItem(newItem);


}

void MainForm::changeEvent(QEvent* e){
    switch (e->type()){
        case QEvent::LanguageChange: this->ui->retranslateUi(this);
            break;
        case QEvent::WindowStateChange:
                if (this->windowState() & Qt::WindowMinimized)
                    QTimer::singleShot(250, this, SLOT(hide()));
                break;
        default://important to have default or you get 100+ warnings
            break;
    }

    QMainWindow::changeEvent(e);
}

void MainForm::MasterBuilds::mousePressEvent(QMouseEvent *event){

    //if it is a normal click just call the parent's mouse press event
    QTreeWidget::mousePressEvent(event);
    QTreeWidget::mouseReleaseEvent(event);

    //the point where the event was fired
    QPoint hotSpot = event->pos();// - child->pos();

    QTreeWidgetItem  *theItem =  itemAt(hotSpot);

    if(!theItem){
        return;//return if it fails to dynamic cast
    }

    QMimeData *mimeData = new QMimeData;

    //set the mime data for mouse drag
    QString indexOfItem = QString::number(this->indexOfTopLevelItem(itemAt(hotSpot)));
    mimeData->setText("#<<MBIndex="+indexOfItem );//+ theItem->text(0));
    mimeData->setData("application/x-hotspot", QByteArray::number(hotSpot.x()) + " " + QByteArray::number(hotSpot.y()));

    QLabel *renderer = new QLabel();
    renderer->setText(theItem->text(0));

    QPixmap pixmap(renderer->size());
    renderer->render(&pixmap); //render the image of the item that will be dragged


    //create a QDrag file to set all the mimedata for drag and drop
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    //don't set hotspot
    //drag->setHotSpot(hotSpot);

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

    /*check whether there is a item with url dropped*/
    QList<QUrl> list = event->mimeData()->urls();

    if(list.size() != 0){//means there is file urls
        QString urlFromMime = list.at(0).toString();

        if(!urlFromMime.contains("file"))
            return;//returns if it is not a file

        //this accept lets the other whatsits name know that the event have been accepted
        if(urlFromMime.contains("file:///"))
            urlFromMime.remove(0,8);
        dropNewBuildAdd(urlFromMime);
        event->accept();
        return;
    }

    /*else looks if a build is dropped from master builds to slave builds*/
    QWidget *slaveTreeWidget = dynamic_cast<QWidget*>(childAt(event->pos()));

    if(slaveTreeWidget){
        if(slaveTreeWidget != ui->groupBoxSlaves)
            return;
        QString buildIndex = event->mimeData()->text();

        //check to see if it is the right item that is dropped
        if(!buildIndex.contains("#<<MBIndex="))
            return;
        dropBuildToSlave(buildIndex, "somethingForNow");
        event->accept();
        return;
    }
    else{
        //qDebug()<<"...FALSE...";
    }

}

void MainForm::dropBuildToSlave(QString fromBuild, QString toSlave){
    //remove this #<<MBIndex=
    fromBuild.remove(0,11);

    int index = -1;

    index = fromBuild.toInt();

    if(index <= -1 || index >= masterBuilds->topLevelItemCount())
        return;

    QString buildname = masterBuilds->topLevelItem(index)->text(0);

    QStringList nameListSuggest;
    for(int i = 0; i < masterBuilds->topLevelItemCount(); i++)
        nameListSuggest<<masterBuilds->topLevelItem(i)->text(0);

    QStringList ipListSuggest;
    for(int i = 0; i < ui->treeWidgetSlaves->topLevelItemCount(); i++)
        ipListSuggest<<ui->treeWidgetSlaves->topLevelItem(i)->text(0);

    CopyBuildOver *copyBuild = new CopyBuildOver(management, nameListSuggest, ipListSuggest,buildname);
    copyBuild->show();


}

void MainForm::dropNewBuildAdd(QString newBuildDirectory){
    AddBuild *newBuild = new AddBuild(management, newBuildDirectory);
    newBuild->show();
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

void MainForm::showOrHideTrayClick(){
    if(!this->isVisible()){
        show();
    }
    else{
        hide();
    }
}

void MainForm::newSlaveConnected(Machine *m, int index){
    //now go find slave and show it if needed
    QTreeWidgetItem *slaveItem = new QTreeWidgetItem();
    slaveItem->setText(0, management->getMachineAt(index)->getMachineIP());
    ui->treeWidgetSlaves->addTopLevelItem(slaveItem);
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0,"ASDAS");
    ui->treeWidgetSlaves->topLevelItem(0)->addChild(item);
}

void MainForm::slaveDisconnected(Machine *m, int index){
    //destroy the tree widget at that index
    ui->treeWidgetSlaves->topLevelItem(index)->~QTreeWidgetItem();
}

void MainForm::initiateAddBuild(Build b){
    management->addBuild(b);
    qDebug()<<"Build added";
    displayBuilds();
}

void MainForm::displayBuilds(){
    qDebug()<<"here to display";
    masterBuilds->clear();
    masterBuilds->setColumnCount(2);
    masterBuilds->hideColumn(1);
    Build* myBuilds = management->getAllBuilds();
    int len = management->getBuildCount();
    qDebug()<<len;
    QTreeWidgetItem *boola;
    QString buildName = "";
    QString strNum;
    int buildNum = -1;
    for(int i = 0; i < len; i++){
        qDebug()<<"looping through builds";
        buildName = myBuilds[i].getBuildName();
        buildNum = myBuilds[i].getBuildID();
        strNum = QString::number(buildNum);
        boola = new QTreeWidgetItem();
        // The build no in column 1, which is hidden
        boola->setText(0,buildName);
        boola->setText(1,strNum);
        masterBuilds->addTopLevelItem(boola);
    }
}

void MainForm::loadXMLBuilds(){
    masterBuilds->clear();
    xmlReader xRead;
    xRead.parseXML();

    QMap<QString,QString> buildsNum = xRead.getBuildNumber();
    QMap<QString,QString> buildsName = xRead.getBuildName();
    QMap<QString,QString> buildsDesc = xRead.getBuildDescription();
    QMap<QString,QString> buildsDir = xRead.getBuildDirectory();

    QMapIterator<QString, QString> i(buildsNum);
    QMapIterator<QString, QString> j(buildsName);
    QMapIterator<QString, QString> k(buildsDesc);
    QMapIterator<QString, QString> l(buildsDir);

    while (i.hasNext() && j.hasNext() && k.hasNext() && l.hasNext())
    {
        i.next(); j.next(); k.next(); l.next();
        Build b(i.value().toInt(),j.value(),k.value(),l.value());
        initiateAddBuild(b);
    }
}

void MainForm::masterBuildsClicked(QModelIndex index){
    QString selBuild = index.sibling(index.row(),1).data().toString();
    qDebug()<<"selBuild:  "<<selBuild;
    int num = selBuild.toInt();
    Build retr = management->getBuildByID(num);
    populateBuildInfo(retr);
    populateTreeWidgetInfo(retr);
}

void MainForm::populateBuildInfo(Build retr){
    buildInfo->clear();
    QTreeWidgetItem *boola;
    for(int i = 0; i < 4; i++){
        boola = new QTreeWidgetItem();
        if(i == 0){
            int bID = retr.getBuildID();
            QString buildID = QString::number(bID);
            boola->setText(0,"Build Number");
            boola->setText(1,buildID);
            boola->setToolTip(0,QString::number(retr.getBuildID()));
            boola->setToolTip(1,QString::number(retr.getBuildID()));
        }
        else if(i == 1){
            boola->setText(0,"Name");
            boola->setText(1,retr.getBuildName());
            boola->setToolTip(0,retr.getBuildName());
            boola->setToolTip(1,retr.getBuildName());
        }
        else if(i == 2){
            boola->setText(0,"Directory");
            boola->setText(1,retr.getBuildDirectory());
            boola->setToolTip(0,retr.getBuildDirectory());
            boola->setToolTip(1,retr.getBuildDirectory());
        }
        else{
            boola->setText(0,"Description");
            boola->setText(1,retr.getBuildDescription());
            boola->setToolTip(0,retr.getBuildDescription());
            boola->setToolTip(1,retr.getBuildDescription());
        }
        buildInfo->addTopLevelItem(boola);
    }
}

void MainForm::populateTreeWidgetInfo(Build retr){
    /*
     *NOTA: Hierdie funksie is vir as mens sou die TreeWidgetInfo met files en hulle mod dates populate
     */

    ui->treeWidgetInfoBox->clear();
    ui->treeWidgetInfoBox->setColumnCount(2);
    QStringList headers;
    headers << "File Name" << "Modification Date";
    ui->treeWidgetInfoBox->setHeaderLabels(headers);
    //Run the DirIterator
    QString dir = retr.getBuildDirectory();
    myDirIterator iter(dir);
    iter.getFileInfo();
    QVector<QString> fileNames = iter.retrieveFileNames();
    QVector<QString> fileMods = iter.retrieveFileMods();
    int amnt = fileNames.size();
    QTreeWidgetItem *boola;
    //Check if dir is empty
    if(amnt <1){
        boola = new QTreeWidgetItem();
        boola->setText(0,"The Directory is empty");
        boola->setText(1,"NA");
        boola->setToolTip(0,"The Directory is empty");
        boola->setToolTip(1,"The Directory is empty");
        ui->treeWidgetInfoBox->addTopLevelItem(boola);
    }else{
        for(int i = 0; i < amnt; i++){
            boola = new QTreeWidgetItem();
            boola->setText(0,fileNames[i]);
            boola->setToolTip(0,fileNames[i]);
            boola->setText(1,fileMods[i]);
            boola->setToolTip(1,fileMods[i]);
            ui->treeWidgetInfoBox->addTopLevelItem(boola);
        }
    }
}

void MainForm::on_actionAdd_Build_triggered(){
    AddBuild *newBuild = new AddBuild(management);
    connect(newBuild,SIGNAL(initiateAddBuild(Build)),this,SLOT(initiateAddBuild(Build)));
    newBuild->show();
}

void MainForm::on_actionCopy_Build_Over_triggered() {

    QStringList nameListSuggest;
    for(int i = 0; i < masterBuilds->topLevelItemCount(); i++)
        nameListSuggest<<masterBuilds->topLevelItem(i)->text(0);

    QStringList ipListSuggest;
    for(int i = 0; i < ui->treeWidgetSlaves->topLevelItemCount(); i++)
        ipListSuggest<<ui->treeWidgetSlaves->topLevelItem(i)->text(0);


    CopyBuildOver *copyBuild = new CopyBuildOver(management, nameListSuggest, ipListSuggest);
    copyBuild->show();
}
