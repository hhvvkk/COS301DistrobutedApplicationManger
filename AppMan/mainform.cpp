#include "mainform.h"
#include "ui_mainform.h"
#include "directoryhandler.h"

MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    management = new Management();
    Server *server = new Server(management);
    management->setServer(server);
    watcher = new Watcher();

    //connect the watcher in order to notify the user and management of changes in builds
    connect(watcher, SIGNAL(mustResynchBuild(int)), this, SLOT(mustResynch(int)));
    connect(watcher, SIGNAL(buildTimerCount(int,int)), this, SLOT(buildTimerCount(int,int)));

    ///connect slots for backward signalling
    connect(management, SIGNAL(newSlaveConnected(Machine*)),this,SLOT(newSlaveConnected(Machine*)));
    connect(management, SIGNAL(slaveDisconnected(int)),this,SLOT(slaveDisconnected(int)));
    connect(management, SIGNAL(slaveGotBuild(Machine*,int, QString,bool)), this, SLOT(slaveGotBuild(Machine*,int, QString,bool)));
    connect(management, SIGNAL(slaveBuildSizeSame(int,int,bool)), this, SLOT(slaveBuildSizeSame(int,int,bool)));

    connect(management, SIGNAL(newSlaveUpdatedBuildName(int,int,QString)), this, SLOT(slaveUpdatedBuildName(int, int, QString)));
    connect(management, SIGNAL(slaveBuildSynched(int,int,double)), this, SLOT(slaveBuildSynched(int,int,double)));
    connect(management, SIGNAL(slaveDeletedBuild(int,int)), this, SLOT(slaveDeletedBuild(int,int)));
    connect(management, SIGNAL(buildDeleted()), this, SLOT(buildDeleted()));

    masterBuilds = new MasterBuilds();
    masterBuilds->setHeaderHidden(true);

    ui->treeWidgetSlaves->hideColumn(1);
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

    /*
     * bottom dockWidget(begin)
     */
    connect(ui->treeWidgetInfoBox, SIGNAL(clicked(QModelIndex)), this, SLOT(infoboxClicked(QModelIndex)));
    /*
     * bottom dockWidget(end)
     */

    //ui->dockWidgetProperty->setLayout();
    connect(masterBuilds, SIGNAL(clicked(QModelIndex)), this, SLOT(masterBuildsClicked(QModelIndex)));

    //connect signals dealing with a timer which will eventually collapse all menus
    collapseTimer = new QTimer();
    collapseTimer->setInterval(10000);
    collapseTimer->stop();
    connect(masterBuilds, SIGNAL(expanded(QModelIndex)), this, SLOT(buildActiveUse()));
    connect(collapseTimer, SIGNAL(timeout()), this, SLOT(collapseMasterBuilds()));

    //load all the builds from the xml
    loadXMLBuilds();

    slaveStats = 0;
    management->startServer();

    /*
     *Create the tray(BEGIN)
     **/
    trayIcon = new QSystemTrayIcon(QIcon(":/images/images/ALogo.png"), this);
    QAction *quitAction = new QAction( "Exit", trayIcon );
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quitTheApplication()));
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

    //set some constant limits
    NAME_SIZE_LIMIT = 25;
    DESCRIPTION_SIZE_LIMIT = 300;
    readupSims();
}

void MainForm::quitTheApplication(){
    //will be connected to the QApplicaiton and will quit when emitting this
    emit quitApplication();
}

MainForm::~MainForm()
{
    if(buildInfo)
        buildInfo->deleteLater();
    delete ui;
    //this must be set to delete later or segmentation fault will occur!
    if(management)
        management->deleteLater();

    if(collapseTimer)
        collapseTimer->deleteLater();
}


MainForm::MasterBuilds::MasterBuilds(QWidget *parent)
    :QTreeWidget(parent){
    setColumnCount(2);
    hideColumn(1);
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

}

void MainForm::dropNewBuildAdd(QString newBuildDirectory){
    AddBuild *newBuild = new AddBuild(newBuildDirectory);
    connect(newBuild, SIGNAL(initiateAddBuild(Build*)), this, SLOT(initiateAddBuild(Build*)));
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

void MainForm::newSlaveConnected(Machine *m){
    //now go find slave and show it if needed
    QTreeWidgetItem *slaveItem = new QTreeWidgetItem();
    slaveItem->setText(0, m->getMachineIP());
    slaveItem->setText(1, QString::number(m->getMachineID()));
    ui->treeWidgetSlaves->addTopLevelItem(slaveItem);
    slaveItem->setBackground(0, Qt::green);//display green, until the slave notifies with builds

    QTreeWidgetItem *activeSimulationItem = new QTreeWidgetItem();
    activeSimulationItem->setText(0, m->getMachineIP());
    activeSimulationItem->setText(1, QString::number(m->getMachineID()));
    ui->treeWidgetActiveSimulations->addTopLevelItem(activeSimulationItem);

    m->getMinStats();
}

void MainForm::slaveDisconnected(int uId){

    //destroy the build treewidget item for a slave
    QString uniqueId = QString::number(uId);

    for(int i = 0; i < ui->treeWidgetSlaves->topLevelItemCount(); i++){
        QTreeWidgetItem *item = 0;
        item = ui->treeWidgetSlaves->topLevelItem(i);
        if(item != 0){
            if(!item->text(1).compare(uniqueId)){
                item->~QTreeWidgetItem();
                break;
            }
        }
    }

    //remove the simulations class for a slave
    for(int i = 0; i < ui->treeWidgetActiveSimulations->topLevelItemCount(); i++){
        QTreeWidgetItem *item = 0;
        item = ui->treeWidgetActiveSimulations->topLevelItem(i);
        if(item != 0){
            if(!item->text(1).compare(uniqueId)){
                item->~QTreeWidgetItem();
                break;
            }
        }
    }
}

void MainForm::initiateAddBuild(Build *newBuild){
    management->addBuild(newBuild);
    watcher->addBuildPath(newBuild->getBuildID(), newBuild->getBuildDirectory());
    displayBuilds();
}

void MainForm::displayBuilds(){
    masterBuilds->clear();
    QList<Build*> myBuilds = management->getAllBuilds();
    int len = management->getBuildCount();
    QTreeWidgetItem *newWidgetItem;
    QString buildName = "";
    QString strID;
    int buildID = -1;
    for(int i = 0; i < len; i++){
        //qDebug()<<"looping through builds";
        buildName = myBuilds[i]->getBuildName();
        buildID = myBuilds[i]->getBuildID();
        strID = QString::number(buildID);
        newWidgetItem = new QTreeWidgetItem();
        // The build no in column 1, which is hidden
        newWidgetItem->setText(0,buildName);
        newWidgetItem->setText(1,strID);
        newWidgetItem->setToolTip(0,strID);
        masterBuilds->addTopLevelItem(newWidgetItem);

        //add the build buttons(copy over, delete, etc.)
        //firstly add the text for the copy over or delete or other "buttons"
        QTreeWidgetItem *copyOverItem = new QTreeWidgetItem();
        copyOverItem->setText(0, "Copy Over");

        QTreeWidgetItem *deleteItem = new QTreeWidgetItem();
        deleteItem->setText(0, "Delete Build");

        QTreeWidgetItem *resynchItem = new QTreeWidgetItem();
        resynchItem->setText(0, "Resynch ");

        //add them both
        newWidgetItem->addChild(copyOverItem);
        newWidgetItem->addChild(deleteItem);
        newWidgetItem->addChild(resynchItem);

    }
}

void MainForm::loadXMLBuilds(){
    masterBuilds->clear();
    xmlReader xRead;
    xRead.parseXML();

    QMap<QString,QString> buildsIDs = xRead.getBuildUniqueID();
    QMap<QString,QString> buildsName = xRead.getBuildName();
    QMap<QString,QString> buildsDesc = xRead.getBuildDescription();
    QMap<QString,QString> buildsDir = xRead.getBuildDirectory();

    QMapIterator<QString, QString> i(buildsIDs);
    QMapIterator<QString, QString> j(buildsName);
    QMapIterator<QString, QString> k(buildsDesc);
    QMapIterator<QString, QString> l(buildsDir);

    while (i.hasNext() && j.hasNext() && k.hasNext() && l.hasNext())
    {
        i.next(); j.next(); k.next(); l.next();
        Build *newBuild = new Build(i.value().toInt(),j.value(),k.value(),l.value());
        initiateAddBuild(newBuild);
    }
}

void MainForm::masterBuildsClicked(QModelIndex index){
    //reset the timer since it is in active use
    buildActiveUse();
    QString selBuild = index.sibling(index.row(),1).data().toString();

    bool ok = false;
    int buildID = selBuild.toInt(&ok);

    if(ok){
        Build *retr = management->getBuildByID(buildID);

        if(buildInfo != 0){
            buildInfo->deleteLater();
        }

        buildInfo = new BuildInfo();

        connect(buildInfo, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(buildInfoDoubleClicked(QTreeWidgetItem*,int)));
        connect(buildInfo, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(buildInfoItemEditedChanged(QTreeWidgetItem*,int)));


        clearDockWidget();
        ui->dockWidgetContents->layout()->addWidget(buildInfo);

        populateBuildInfo(retr);
        populateTreeWidgetInfo(retr);
    }
    else{
        //else if it has failed, it most likely is a action button clicked(CopyOver, Delete, etc.)
        QString itemClicked = index.data().toString();

        if(!itemClicked.compare(""))
            return;

        if(!itemClicked.compare("Copy Over")){//meaning a copy over will occur
            //int itemIndex = index.parent().row();
            QString buildName = index.parent().sibling(index.parent().row(), 0).data().toString();

            QStringList nameListSuggest;
            for(int i = 0; i < masterBuilds->topLevelItemCount(); i++){
                nameListSuggest<<masterBuilds->topLevelItem(i)->text(0);
            }
            QStringList ipListSuggest;
            QTreeWidgetItem *safetyItem;//so that if that item is retrieved, but does not exist(concurrency)
            for(int i = 0; i < ui->treeWidgetSlaves->topLevelItemCount(); i++){
                safetyItem = ui->treeWidgetSlaves->topLevelItem(i);
                if(safetyItem != 0)
                    ipListSuggest<< safetyItem->text(0);
            }
            CopyBuildOver *copyBuild = new CopyBuildOver(management, nameListSuggest, ipListSuggest, buildName);
            copyBuild->show();
            connect(copyBuild, SIGNAL(copyBuildOver(int,QString)), this, SLOT(initiateCopyBuildOver(int,QString)));
        }
        else if(!itemClicked.compare("Delete Build")){
            //int itemIndex = index.parent().row();
            QString buildID = index.parent().sibling(index.parent().row(), 1).data().toString();

            bool convertParentBID  = false;
            int convertedID = buildID.toInt(&convertParentBID);

            if(!convertParentBID)
                return;

            watcher->removeBuildPath(convertedID);

            management->deleteBuild(convertedID);

            buildInfo->clear();
        }else if(itemClicked.contains("Resynch ")){
            QString buildID = index.parent().sibling(index.parent().row(), 1).data().toString();

            bool convertParentBID  = false;
            int convertedID = buildID.toInt(&convertParentBID);

            if(!convertParentBID)
                return;

            watcher->stopCountDown(convertParentBID);

            //notify through management to resynch this build on all slaves
            management->resynchAllCertainBuild(convertedID);
        }
    }
}

void MainForm::populateBuildInfo(Build *retr){

    if(retr == 0)
        return;

    buildInfo->show();
    buildInfo->clear();
    QTreeWidgetItem *boola;
    for(int i = 0; i < 4; i++){
        boola = new QTreeWidgetItem();

        if(i == 0){
            int bID = retr->getBuildID();
            QString buildID = QString::number(bID);
            boola->setText(0,"BuildID");
            boola->setText(1,buildID);
            boola->setToolTip(0,QString::number(retr->getBuildID()));
            boola->setToolTip(1,QString::number(retr->getBuildID()));
        }
        else if(i == 1){
            boola->setText(0,"Name");
            boola->setText(1,retr->getBuildName());
            boola->setToolTip(0,retr->getBuildName());
            boola->setToolTip(1,retr->getBuildName());
        }
        else if(i == 2){
            boola->setText(0,"Directory");
            boola->setText(1,retr->getBuildDirectory());
            boola->setToolTip(0,retr->getBuildDirectory());
            boola->setToolTip(1,retr->getBuildDirectory());
        }
        else{
            boola->setText(0,"Description");
            boola->setText(1,retr->getBuildDescription());
            boola->setToolTip(0,retr->getBuildDescription());
            boola->setToolTip(1,retr->getBuildDescription());
        }
        buildInfo->addTopLevelItem(boola);
    }
}

void MainForm::populateTreeWidgetInfo(Build *retr){
    /*
     *NOTA: Hierdie funksie is vir as mens sou die TreeWidgetInfo met files en hulle mod dates populate
     */
    ui->treeWidgetInfoBox->clear();
    ui->treeWidgetInfoBox->setColumnCount(2);
    QStringList headers;
    headers << "File Name" << "Modification Date";
    ui->treeWidgetInfoBox->setHeaderLabels(headers);
    //Run the DirIterator
    QString dir = retr->getBuildDirectory();
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
    AddBuild *newBuild = new AddBuild();
    connect(newBuild,SIGNAL(initiateAddBuild(Build*)),this,SLOT(initiateAddBuild(Build*)));
    newBuild->show();
}

void MainForm::on_actionCopy_Build_Over_triggered() {
    QStringList nameListSuggest;
    for(int i = 0; i < masterBuilds->topLevelItemCount(); i++){
        nameListSuggest<<masterBuilds->topLevelItem(i)->text(0);
    }
    QStringList ipListSuggest;
    QTreeWidgetItem *safetyItem;//so that if that item is retrieved, but does not exist(concurrency)
    for(int i = 0; i < ui->treeWidgetSlaves->topLevelItemCount(); i++){
        safetyItem = ui->treeWidgetSlaves->topLevelItem(i);
        if(safetyItem != 0)
            ipListSuggest<< safetyItem->text(0);
    }
    CopyBuildOver *copyBuild = new CopyBuildOver(management, nameListSuggest, ipListSuggest);
    copyBuild->show();
    connect(copyBuild, SIGNAL(copyBuildOver(int,QString)), this, SLOT(initiateCopyBuildOver(int,QString)));
}

void MainForm::initiateCopyBuildOver(int uniqueId, QString buildName){
    management->copyBuildOver(uniqueId, buildName);
}

void MainForm::slaveGotBuild(Machine*m, int buildId,  QString slaveBuildName, bool buildExist){
    QTreeWidgetItem *machineT = getSlaveTreeItemById(m->getMachineID());

    if(machineT == 0){
        return;
    }

    //firstly check whether it already exists or not
    //if it does exist, return(don't want dupilicate values)
    for(int i = 0; i < machineT->childCount(); i++){
        QTreeWidgetItem *childItem = machineT->child(i);

        bool ok = false;

        int theBuildID = childItem->text(1).toInt(&ok);

        if(ok){
            if(theBuildID == buildId)
                return;//dont want duplicate items
        }
        else{
            return;
        }
    }

    QString slaveBuildId = QString::number(buildId);
    QTreeWidgetItem *newBuildForSlave = new QTreeWidgetItem();
    if(buildExist == false){
        //show the background colour as red as well as display that buildId
        newBuildForSlave->setBackgroundColor(0, Qt::red);
        newBuildForSlave->setText(0, slaveBuildName );
        newBuildForSlave->setText(1,slaveBuildId);
    }
    else{
        newBuildForSlave->setText(0, slaveBuildName);
        newBuildForSlave->setText(1,slaveBuildId);
        newBuildForSlave->setToolTip(0, "0% Synched");
    }
    machineT->addChild(newBuildForSlave);
}


QTreeWidgetItem* MainForm::getSlaveTreeItemById(int uniqueId){
    QTreeWidgetItem * machineTreeItem = 0;
    QTreeWidgetItem *safetyItem;//so that if that item is retrieved, but does not exist(concurrency)
    for(int i = 0; i < ui->treeWidgetSlaves->topLevelItemCount(); i++){
        safetyItem = ui->treeWidgetSlaves->topLevelItem(i);
        if(safetyItem != 0)
            if(!safetyItem->text(1).compare(QString::number(uniqueId))){
                machineTreeItem = ui->treeWidgetSlaves->topLevelItem(i);
                break;
            }
    }
    return machineTreeItem;
}

void MainForm::slaveBuildSizeSame(int buildId, int machineId, bool isTheSame){
    QTreeWidgetItem * slaveTreeWidgetItem = getSlaveTreeItemById(machineId);
    QTreeWidgetItem * buildItem = 0;

    QString theBuildId = QString::number(buildId);

    if(slaveTreeWidgetItem == 0)
        return;

    for(int i = 0; i < slaveTreeWidgetItem->childCount(); i++){
        if(!slaveTreeWidgetItem->child(i)->text(1).compare(theBuildId)){
            buildItem = slaveTreeWidgetItem->child(i);
            break;
        }
    }

    if(buildItem == 0){
        return;
    }

    QBrush good = Qt::green;
    QBrush bad = Qt::yellow;

    if(isTheSame){
        buildItem->setBackground(0, good);
        buildItem->setToolTip(0, "100%");
    }
    else{
        buildItem->setBackground(0, bad);
    }

    //check whether the slave is fully synchronised(if so, display green, else yellow)
    bool displayGreen = true;
    for(int i = 0; i < slaveTreeWidgetItem->childCount(); i++){
        QTreeWidgetItem *anItem = slaveTreeWidgetItem->child(i);

        if(anItem->background(0).color() == bad.color()){
            displayGreen = false;
            break;
        }
    }

    if(displayGreen){
        slaveTreeWidgetItem->setBackground(0, good);
        QString zipStuffToDelete = getCompressPath();
        zipStuffToDelete.append("/"+QString::number(machineId));
        DirectoryHandler::removeDir(zipStuffToDelete);
        slaveTreeWidgetItem->setToolTip(0, "fully synched");
    }
    else{
        slaveTreeWidgetItem->setBackground(0, bad);
        slaveTreeWidgetItem->setToolTip(0, "synchronising");
    }
}

QString MainForm::getCompressPath(){
    QString fileCompressPath = "buildCompressed";
    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("FileCopy");

    //default settings values
    QVariant defaultfileCompressPath;
    QString theDefaultPathCompress = "buildCompressed";
    defaultfileCompressPath.setValue(theDefaultPathCompress);

    //send in the default values in case it does not exist...
    QString loadedFileCompressPath = setting.value("fileCompressPath", defaultfileCompressPath).toString();

    setting.endGroup();

    if(!QDir(loadedFileCompressPath).exists()){
        //firstly try and create that directory...
        bool successCreate = QDir().mkdir(loadedFileCompressPath);
        //...if it fails, revert to default
        if(!successCreate){
            fileCompressPath = "buildCompressed";
            QDir().mkdir(fileCompressPath);
        }
    }
    else{
        fileCompressPath = loadedFileCompressPath;
    }

    return fileCompressPath;
}


void MainForm::slaveBuildSynched(int machineId, int buildId, double percentage){
   // qDebug()<<"slaveSynched[machineID = "<< machineId <<"|| buildID = "<< buildId <<"] = "<<percentage;
    QTreeWidgetItem *slaveItem = getSlaveTreeItemById(machineId);

    if(slaveItem == 0){
        return;
    }

    for(int i = 0; i < slaveItem->childCount(); i++){
        QTreeWidgetItem *slaveChild = slaveItem->child(i);

        bool ok = false;


        int slaveBuildID  = slaveChild->text(1).toInt(&ok);

        if(!ok)
            continue;

        if(slaveBuildID == buildId){
            //means the correct one is found
            QString percentSynched = QString::number(percentage) + "%";
            slaveChild->setToolTip(0, percentSynched);
            break;
        }
    }

    //set the other item to see how far it is synched
    //if and only if it is shown...

    for(int i = 0; i < ui->treeWidgetInfoBox->topLevelItemCount(); i++){
        QTreeWidgetItem *item = ui->treeWidgetInfoBox->topLevelItem(i);
        if(item == 0)
            continue;
        if(item->text(0).contains("Synched")){
            item->setText(1, QString::number(percentage) + "%");
        }
    }
}


MainForm::SlaveStats::SlaveStats(QWidget *parent, int machineID, QString input)
    :QTreeWidget(parent){
    QString labelHeader1 = "Property";
    QString labelHeader2 = "Value";
    QStringList labelHeaders;
    labelHeaders << labelHeader1<<labelHeader2;
    QTreeWidget::setColumnCount(3);
    QTreeWidget::setHeaderLabels(labelHeaders);
    QStringList lists = input.split("#");
    QTreeWidgetItem *newItem;
    this->hideColumn(2);

    newItem = new QTreeWidgetItem();
    newItem->setText(0,"CPU usage %");
    newItem->setText(1,lists.at(0));
    newItem->setText(2,QString::number(machineID));
    this->addTopLevelItem(newItem);

    newItem = new QTreeWidgetItem();
    newItem->setText(0,"RAM usage %");
    newItem->setText(1,lists.at(1));
    newItem->setText(2,QString::number(machineID));
    this->addTopLevelItem(newItem);


    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Data Transmitted");
    newItem->setText(1,lists.at(2));
    newItem->setText(2,QString::number(machineID));
    this->addTopLevelItem(newItem);


    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Data Received");
    newItem->setText(1,lists.at(3));
    newItem->setText(2,QString::number(machineID));
    this->addTopLevelItem(newItem);

    newItem = new QTreeWidgetItem();
    newItem->setText(0,"Click for more");
    newItem->setText(1,"More");
    newItem->setText(2, QString::number(machineID));
    this->addTopLevelItem(newItem);
}

void MainForm::on_treeWidgetSlaves_clicked(const QModelIndex &index)
{
    slaveItemClicked(index);
}

void MainForm::slaveItemClicked(const QModelIndex &index){
    QTreeWidgetItem *item = ui->treeWidgetSlaves->selectedItems().at(0);

    if(item->parent() != 0){
        //this means that the item is not a top level item, thus the slave's build
        //(since the item has a parent which parent() != 0
        QTreeWidgetItem *itemsParent = item->parent();
        bool ok = false;


        int machineID = itemsParent->text(1).toInt(&ok);

        if(!ok)
            return;

        displaySlaveBuildInfo(item, machineID);
        return;
    }

    bool *ok = new bool();
    int machineID = item->text(1).toInt();

    if(!ok){
        delete ok;
        return;
    }
    delete ok;

    Machine * selected = management->getMachineById(machineID);

    if(selected == 0)
        return;
    selected->getMinStats();
    //QString inp = "16%#46%#2.39695MB#5.375KB" ;
    buildInfo->hide();
    if(slaveStats != 0){
        slaveStats->deleteLater();
    }
    slaveStats = new SlaveStats(this,selected->getMachineID(), selected->getMinStatsString());
    connect(slaveStats, SIGNAL(clicked(QModelIndex)), this, SLOT(slaveStatsClicked(QModelIndex)));
    clearDockWidget();
    ui->dockWidgetContents->layout()->addWidget(slaveStats);
}

void MainForm::clearDockWidget(){
    int cnt = ui->dockWidgetContents->layout()->count();
    for(int i = 0; i < cnt; i++){
        QWidget *w = ui->dockWidgetContents->layout()->itemAt(i)->widget();
        ui->dockWidgetContents->layout()->removeWidget(w);
    }
}

void MainForm::slaveStatsClicked(QModelIndex index){
    QTreeWidgetItem *contentBoxSelected = slaveStats->topLevelItem(index.row());
    QString selectedText = contentBoxSelected->text(0);

    if(selectedText.compare("Click for more")){
        //if you dont want more information(i.e. clicked another place)
        return;
    }

    QString stringID = contentBoxSelected->text(2);

    bool *ok = new bool();
    int machineID = stringID.toInt(ok);

    if(!ok){
        delete ok;
        return;
    }

    delete ok;


    Machine* selected = management->getMachineById(machineID);
    //selected->getDetStats();
    moreInfo* mi = new moreInfo(selected);
    mi->show();    
}

void MainForm::slaveUpdatedBuildName(int machineID, int buildID, QString updatedName){
    QTreeWidgetItem *item = 0;

    item = getSlaveTreeItemById(machineID);

    if(item == 0)
        return;

    QTreeWidgetItem *buildChild = 0;

    for(int i = 0; i < item->childCount(); i++){
        if(!item->child(i)->text(1).compare(QString::number(buildID))){
            buildChild = item->child(i);
        }
    }

    if(buildChild == 0)
        return;
    buildChild->setText(0, updatedName);

}

void MainForm::buildInfoDoubleClicked(QTreeWidgetItem* theDoubleClickedItem, int theColumn){

    if(!theDoubleClickedItem->text(0).compare("Directory")){
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, "Change build directory", "Are you sure you wish to change the directory?", QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {

            if(buildInfo == 0)
                return;

            int indexBuildID = -1;
            for(int i = 0; i < buildInfo->topLevelItemCount();i++){
                //find out what the buildID is that is currently looked for
                if(!buildInfo->topLevelItem(i)->text(0).compare("BuildID")){
                    indexBuildID = i;
                    break;
                }
            }


            if(indexBuildID <= -1)
                return;

            //change the number into an actual integer
            bool ok = false;

            int buildID = buildInfo->topLevelItem(indexBuildID)->text(1).toInt(&ok);

            if(!ok)
                return;

            QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"), theDoubleClickedItem->text(1), QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);

            if(!QDir().exists(directory))
                return;

            setBuildInfo(BUILDDIRECTORY, directory, buildID);
        }
        return;
    }
    if(theColumn == 1){
        if(!theDoubleClickedItem->text(0).compare("BuildID"))
            return;
        buildInfo->openPersistentEditor(theDoubleClickedItem,theColumn);
    }
}

void MainForm::buildInfoItemEditedChanged(QTreeWidgetItem* itemChanged, int column){
    buildInfo->closePersistentEditor(itemChanged, column);
    QString value = itemChanged->text(1);

    QString setWhatQString = itemChanged->text(0);

    int setWhat = -1;

    //directory is set some other way, not like this...
    if(!setWhatQString.compare("Name")){
        setWhat = BUILDNAME;
    }else if(!setWhatQString.compare("Number")){
        setWhat = BUILDNUMBER;
    }else if(!setWhatQString.compare("Description")){
        setWhat = BUILDDESCRIPTION;
    }

    //get the build ID...
    if(buildInfo == 0)
        return;

    int indexBuildID = -1;
    for(int i = 0; i < buildInfo->topLevelItemCount();i++){
        //find out what the buildID is that is currently looked for
        if(!buildInfo->topLevelItem(i)->text(0).compare("BuildID")){
            indexBuildID = i;
            break;
        }
    }


    if(indexBuildID <= -1)
        return;

    //change the number into an actual integer
    bool ok = false;

    int buildID = buildInfo->topLevelItem(indexBuildID)->text(1).toInt(&ok);

    if(!ok)
        return;

    setBuildInfo(setWhat, value, buildID);
}

void MainForm::setBuildInfo(int setWhat, QString value, int buildID){
    if(management == 0)
        return;

    if(setWhat == BUILDDIRECTORY){
        //gui interface already changed
        //thus only set the xml
        management->setBuildDirectory(buildID, value);

        watcher->changeDirToWatch(buildID, value);

        QTreeWidgetItem *treeWidgItem = 0;

        int count = 0;

        if(buildInfo == 0)
            return;

        while(count <= buildInfo->topLevelItemCount()){
            //find the directory item
            if(!buildInfo->topLevelItem(count)->text(0).compare("Directory")){
                treeWidgItem = buildInfo->topLevelItem(count);
                break;
            }
            count++;
        }

        if(treeWidgItem == 0){
            return;//if it has not found it return
        }

        treeWidgItem->setText(1, value);
        treeWidgItem->setToolTip(1, value);
        treeWidgItem->setToolTip(0, value);
    }
    else if(setWhat == BUILDNAME){
        if(value.size() > NAME_SIZE_LIMIT || value.size() < 1){
            //don't allow sizes larger than 25 for names or size of 0
            QPixmap pic(":/images/images/ALogo.png");
            QMessageBox *messageBox = new QMessageBox();
            messageBox->setText("Error: The allowed range for build name is between 1 & "+QString::number(NAME_SIZE_LIMIT));
            messageBox->setWindowIcon(QIcon(pic));
            messageBox->show();
            return;
        }

        if(management->buildExistWithName(value)){
            return;
        }

        //set the buildinfo in gui
        for(int i = 0; i < masterBuilds->topLevelItemCount(); i++){
            if(!masterBuilds->topLevelItem(i)->text(1).compare(QString::number(buildID))){
                masterBuilds->topLevelItem(i)->setText(0, value);
                break;
            }
        }
        //set the buildInfo in xml
        management->setBuildName(buildID, value);

        //finally set the build name tooltips
        QTreeWidgetItem *treeWidgItem = 0;

        int count = 0;

        if(buildInfo == 0)
            return;

        while(count <= buildInfo->topLevelItemCount()){
            //find the directory item
            if(!buildInfo->topLevelItem(count)->text(0).compare("Name")){
                treeWidgItem = buildInfo->topLevelItem(count);
                break;
            }
            count++;
        }

        if(treeWidgItem == 0){
            return;//if it has not found it return
        }

        treeWidgItem->setToolTip(1, value);
        treeWidgItem->setToolTip(0, value);
    }
    else if(setWhat == BUILDNUMBER){
        for(int i = 0; i < masterBuilds->topLevelItemCount(); i++){
            if(!masterBuilds->topLevelItem(i)->text(1).compare(QString::number(buildID))){
                masterBuilds->topLevelItem(i)->setToolTip(0, value);
                break;
            }
        }

        //set the buildInfo in xml
        management->setBuildNumber(buildID, value);


        //finally set the build name tooltips
        QTreeWidgetItem *treeWidgItem = 0;

        int count = 0;

        if(buildInfo == 0)
            return;

        while(count <= buildInfo->topLevelItemCount()){
            //find the directory item
            if(!buildInfo->topLevelItem(count)->text(0).compare("BuildNumber")){
                treeWidgItem = buildInfo->topLevelItem(count);
                break;
            }
            count++;
        }

        if(treeWidgItem == 0){
            return;//if it has not found it return
        }

        treeWidgItem->setToolTip(1, value);
        treeWidgItem->setToolTip(0, value);
    }
    else if(setWhat == BUILDDESCRIPTION){
        if(value.size() > DESCRIPTION_SIZE_LIMIT){
            //don't allow sizes larger than 25 for names or size of 0
            return;
        }


        //no gui elements for this
        //set the buildInfo in xml
        management->setBuildDescription(buildID, value);


        //finally set the build name tooltips
        QTreeWidgetItem *treeWidgItem = 0;

        int count = 0;

        if(buildInfo == 0)
            return;

        while(count <= buildInfo->topLevelItemCount()){
            //find the directory item
            if(!buildInfo->topLevelItem(count)->text(0).compare("Description")){
                treeWidgItem = buildInfo->topLevelItem(count);
                break;
            }
            count++;
        }

        if(treeWidgItem == 0){
            return;//if it has not found it return
        }

        treeWidgItem->setToolTip(1, value);
        treeWidgItem->setToolTip(0, value);
    }
}

void MainForm::on_actionAdd_Simulation_triggered(){
    AddSimulation * as = new AddSimulation(this,management);
     connect(as,SIGNAL(initiateAddSimulation(Simulation *)),this,SLOT(initiateAddSimulation(Simulation *)));
     as->show();
}

void MainForm::initiateAddSimulation(Simulation *sim){
    management->addSimulation(sim);
    showSimulations();
}

void MainForm::showSimulations(){
    ui->treeWidgetSimulations->clear();
        ui->treeWidgetSimulations->setColumnCount(1);
        QTreeWidgetItem *newItem;
        for(int i = 0; i < management->getSimCount(); i++){
            newItem = new QTreeWidgetItem();
            newItem->setText(0,management->getAllSims().at(i)->getName());
            ui->treeWidgetSimulations->addTopLevelItem(newItem);
        }
}

void MainForm::buildActiveUse(){
    //reset the timer which will clear the expanding of all items in masterBuilds
    collapseTimer->stop();
    collapseTimer->start();
}

void MainForm::collapseMasterBuilds(){
    if(mouseCurserOver(masterBuilds)){
        //then the item is most probably still active
        buildActiveUse();
    }else{//then it is most likely not in use
        masterBuilds->collapseAll();
        masterBuilds->clearSelection();
        //stop the timer since it is done!
        collapseTimer->stop();
    }

}

bool MainForm::mouseCurserOver(QWidget *theItem){
    //returns true if the mouse curser is over the sitem sent through
    if(!theItem)
        return false;

    if(theItem->underMouse())
        return true;
    else
        return false;
}

void MainForm::slaveDeletedBuild(int machineID, int buildID){
    QTreeWidgetItem *slaveItem = getSlaveTreeItemById(machineID);

    if(slaveItem == 0)
        return;

    //the Child count is how many builds there are
    for(int i = 0; i < slaveItem->childCount(); i++){
        QTreeWidgetItem *childItem = slaveItem->child(i);
        if(childItem != 0){
            //the buildID is the 2nd column(i.e. index 1)
            bool ok = false;

            int theItemBuildID = childItem->text(1).toInt(&ok);

            if(ok){
                if(theItemBuildID == buildID){
                    //delete it since it is removed
                    childItem->~QTreeWidgetItem();
                    break;
                }
            }
        }
    }
}

void MainForm::buildDeleted(){
    displayBuilds();
}

void MainForm::displaySlaveBuildInfo(QTreeWidgetItem *slaveBuildItem, int machineID){
    ui->treeWidgetInfoBox->clear();
    ui->treeWidgetInfoBox->setColumnCount(2);

    QStringList headers;
    headers << "Property" << "Value";
    ui->treeWidgetInfoBox->setHeaderLabels(headers);


    QTreeWidgetItem *machineInfoID = new QTreeWidgetItem();
    machineInfoID->setText(0, "Machine ID");
    machineInfoID->setText(1, QString::number(machineID));
    ui->treeWidgetInfoBox->addTopLevelItem(machineInfoID);

    QTreeWidgetItem *slaveBuildInfo = new QTreeWidgetItem();
    slaveBuildInfo->setText(0, "Build ID");
    slaveBuildInfo->setText(1, slaveBuildItem->text(1));
    ui->treeWidgetInfoBox->addTopLevelItem(slaveBuildInfo);

    QTreeWidgetItem *synchInformation = new QTreeWidgetItem();
    synchInformation->setText(0, "Synched");
    synchInformation->setText(1, slaveBuildItem->toolTip(0));
    ui->treeWidgetInfoBox->addTopLevelItem(synchInformation);


    QTreeWidgetItem *deleteOrResynch = new QTreeWidgetItem();
    deleteOrResynch->setText(0, "Delete Build");
    deleteOrResynch->setText(1, "Resynch Build");
    ui->treeWidgetInfoBox->addTopLevelItem(deleteOrResynch);
    deleteOrResynch->setBackground(0, QColor("#B2CCFF"));
    deleteOrResynch->setBackground(1, QColor("#B2CCFF"));
}


void MainForm::infoboxClicked(QModelIndex index){
    QTreeWidgetItem *item = ui->treeWidgetInfoBox->topLevelItem(index.row());
    QString clickedItemText = item->text(index.column());

    if(!clickedItemText.compare("Delete Build") || !clickedItemText.compare("Resynch Build")){
        QString stringMachineID = "";
        QString stringBuildID = "";
        for(int i = 0; i < ui->treeWidgetInfoBox->topLevelItemCount(); i++){
            QTreeWidgetItem *item = ui->treeWidgetInfoBox->topLevelItem(i);
            if(item != 0){
                if(!item->text(0).compare("Machine ID"))
                    stringMachineID = item->text(1);
                if(!item->text(0).compare("Build ID"))
                    stringBuildID = item->text(1);
            }
        }

        if(!stringMachineID.compare("") || !stringBuildID.compare("")  ){
            return;
        }

        bool ok = false;

        int buildID = stringBuildID.toInt(&ok);

        if(!ok)
            return;

        int machineID = stringMachineID.toInt(&ok);

        if(!ok)
            return;

        if(!clickedItemText.compare("Delete Build")){
            management->deleteBuildFromSlave(machineID, buildID);
            ui->treeWidgetInfoBox->clear();
        }
        if(!clickedItemText.compare("Resynch Build")){
            management->resynchAllBuildsOnSlave(machineID);
        }

    }
}

void MainForm::mustResynch(int buildID){
    management->resynchAllCertainBuild(buildID);
    buildTimerCount(buildID, 0);
}

void MainForm::buildTimerCount(int buildID, int timeRemaining){
    for(int i = 0; i < masterBuilds->topLevelItemCount(); i++){
        QTreeWidgetItem *item = masterBuilds->topLevelItem(i);
        bool ok = false;


        int itemBuildID = item->text(1).toInt(&ok);

        if(!ok)
            continue;

        if(itemBuildID == buildID){
            //find its child and change the resynch value
            for(int j = 0; j < item->childCount(); j++){
                QTreeWidgetItem *childItem = item->child(j);

                if(childItem == 0){
                    continue;
                }

                QString childTextItem = childItem->text(0);

                if(childTextItem.contains("Resynch")){
                    if(timeRemaining <= 0)
                        childItem->setText(0, "Resynch ");
                    else
                        childItem->setText(0, "Resynch ("+QString::number(timeRemaining)+")");
                    buildActiveUse();
                }
            }
            break;
        }
    }
}

void MainForm::readupSims(){
    simXMLReader xRead;
    for(int i = 0; i < xRead.getSimNames().length(); i++){
        Simulation *sim = new Simulation(xRead.getSimNames().at(i));
        sim->addSlave(xRead.getReqSlaves().value(i),xRead.getReqBuilds().value(i),xRead.getReqArgs().value(i));
        management->addSimulation(sim);
    }
    showSimulations();
}
void MainForm::on_treeWidgetSimulations_clicked(const QModelIndex &index)
{
    QTreeWidgetItem *item = ui->treeWidgetSimulations->selectedItems().at(0);
        QString simName = item->text(0);
        management->runSimulation(simName);
}
