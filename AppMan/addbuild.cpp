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

    //Exit if a field is empty
    if(BuildID.compare("") == 0|| buildName.compare("") == 0){
        showError("All fields are required","error");
        return;
    }

    //Exit if the BuildID is in use
    if(!checkBuildID(BuildID)){
        showError("The Build Number cannot be a duplicate entry","error");
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

bool AddBuild::checkBuildID(QString buildNum){
    xmlReader xRead;
    xRead.parseXML();
    bool original = true;
    QMap<QString,QString> buildNums = xRead.getBuildNumber();
    QMapIterator<QString, QString> i(buildNums);
    while (i.hasNext())
    {
        i.next();
        if(i.value().compare(buildNum)==0){
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

void AddBuild::addToXML(int num,QString name,QString descript,QString direc){
    QString bNum = QString::number(num);
    theXMLWriter->receiveBuild(bNum,name,descript,direc);
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
