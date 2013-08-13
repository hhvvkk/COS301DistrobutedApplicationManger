#include "addbuild.h"
#include "ui_addbuild.h"

AddBuild::AddBuild(Management *man, QString directory, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddBuild),
    management(man)
{
    ui->setupUi(this);
    ui->lineEditDirectory->setText(directory);
    theXMLWriter = new xmlWriter();
}

AddBuild::~AddBuild()
{
    delete ui;
}

void AddBuild::cancelClick(){
    this->close();
}

void AddBuild::okClick(){
    QString buildDirectory = ui->lineEditDirectory->text();
    //Check to see if directory actually exist...
    if(!QDir(buildDirectory).exists()){
        showError("Invalid directory path");
        return;
    }
    //continue if it does exist
    QString buildNo = ui->lineEditBuildNo->text();
    QString buildName = ui->lineEditName->text();
    QString buildDescription = ui->lineEditDescription->text();


    int buildID = buildNo.toInt();
    Build buildToAdd(buildID,buildName,buildDescription,buildDirectory);
    qDebug()<<"about to emit";
    addToXML(buildID,buildName,buildDescription,buildDirectory);
    ui->lineEditBuildNo->setText("");
    ui->lineEditName->setText("");
    ui->lineEditDescription->setText("");
    ui->lineEditDirectory->setText("");
    ui->lineEditDirectory->setFocus();


    emit initiateAddBuild(buildToAdd);
}

void AddBuild::showError(QString errorMessage){
    QMessageBox *msb = new QMessageBox();
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
