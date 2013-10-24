#include "addbuild.h"
#include "ui_addbuild.h"

AddBuild::AddBuild(QString directory, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddBuild)
{
    ui->setupUi(this);
    #ifdef WIN32
        ui->lineEditDirectory->setText(directory);
    #else
        ui->lineEditDirectory->setText("/"+directory);
    #endif

    theXMLWriter = new xmlWriter();

    //set some constant limits
    NAME_SIZE_LIMIT = 25;
    DESCRIPTION_SIZE_LIMIT = 300;

}

AddBuild::~AddBuild()
{
    delete ui;
    delete theXMLWriter;
}

void AddBuild::cancelClick(){
    this->close();
}

void AddBuild::okClick(){
    QString buildDirectory = ui->lineEditDirectory->text();
    //Check to see if directory actually exist...
    if(!QDir(buildDirectory).exists()){
        showError("Invalid directory path","error");
        return;
    }
    //continue if it does exist
    QString BuildID = ui->lineEditBuildID->text();
    QString buildName = ui->lineEditName->text();
    QString buildDescription = ui->lineEditDescription->text();

    if(buildName.size() > NAME_SIZE_LIMIT || !buildName.compare("")){
        showError("Build name not within valid range(1-25)","error");
    }

    //Exit if a field is empty
    if(BuildID.compare("") == 0|| buildName.compare("") == 0){
        showError("All fields are required","error");
        return;
    }

    //Exit if the BuildID is in use
    if(!checkBuildID(BuildID)){
        showError("The Build ID cannot contain a duplicate entry","error");
        return;
    }
    if(!checkBuildName(buildName)){
        showError("There already exist a build with that name","error");
        return;
    }

    //If all is fine, continue
    int buildID = BuildID.toInt();
    Build * newBuild = new Build(buildID,buildName,buildDescription,buildDirectory);
    //qDebug()<<"about to emit";
    addToXML(buildID,buildName,buildDescription,buildDirectory);

    //clean up for next add
    ui->lineEditBuildID->setText("");
    ui->lineEditName->setText("");
    ui->lineEditDescription->setText("");
    ui->lineEditDirectory->setText("");
    ui->lineEditDirectory->setFocus();

    //Use show error to confirm the build is added
    showError("Build added successfully","info");

    emit initiateAddBuild(newBuild);
    this->close();
}

bool AddBuild::checkBuildID(QString buildID){
    xmlReader xRead;
    xRead.parseXML();
    bool original = true;
    QMap<QString,QString> buildIDs = xRead.getBuildUniqueID();
    QMapIterator<QString, QString> i(buildIDs);
    while (i.hasNext())
    {
        i.next();
        if(i.value().compare(buildID)==0){
            original = false;
            break;
        }
        else original = true;
    }
    return original;
}

bool AddBuild::checkBuildName(QString buildName){
    xmlReader xRead;
    xRead.parseXML();
    bool original = true;
    QMap<QString,QString> buildNames = xRead.getBuildName();
    QMapIterator<QString, QString> i(buildNames);
    while (i.hasNext())
    {
        i.next();
        if(i.value().compare(buildName)==0){
            original = false;
            break;
        }
        else original = true;
    }
    return original;
}

void AddBuild::showError(QString errorMessage, QString info){
    QMessageBox *msb = new QMessageBox();
    if(info.compare("error") == 0){
        msb->setIcon(QMessageBox::Critical);
    }
    if(info.compare("inform") == 0){
        msb->setIcon(QMessageBox::Information);
    }
    QPixmap pic(":/images/images/ALogo.png");
    //msb->setIconPixmap(pic.scaled(50,50,Qt::IgnoreAspectRatio,Qt::FastTransformation));
    msb->setWindowIcon(QIcon(pic));
    msb->setText(errorMessage);
    msb->show();
}

void AddBuild::addToXML(int ID,QString name,QString descript,QString direc){
    QString bID = QString::number(ID);
    theXMLWriter->receiveBuild(bID,name,descript,direc);
    theXMLWriter->createXMLFile();
}

void AddBuild::chooseClick(){
    QString directory = "";
    if(ui->lineEditDirectory->text().compare("")){
        directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),ui->lineEditDirectory->text(),QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    }
    else{
        directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    }

    if(directory.compare(""))
        ui->lineEditDirectory->setText(directory);
}
