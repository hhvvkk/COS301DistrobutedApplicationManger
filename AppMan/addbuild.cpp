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
    QString buildNo = ui->lineEditBuildNo->text();
    QString buildName = ui->lineEditName->text();
    QString buildDescription = ui->lineEditDescription->text();
    QString buildDirectory = ui->lineEditDirectory->text();
    int buildID = buildNo.toInt();
    Build buildToAdd(buildID,buildName,buildDescription,buildDirectory);
    qDebug()<<"about to emit";
    addToXML(buildID,buildName,buildDescription,buildDirectory);
    emit initiateAddBuild(buildToAdd);
}

void AddBuild::addToXML(int num,QString name,QString descript,QString direc){
    QString bNum = QString::number(num);
    theXMLWriter->receiveBuild(bNum,name,descript,direc);
    theXMLWriter->CreateXMLFile();

}

void AddBuild::chooseClick(){
    QString directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    ui->lineEditDirectory->setText(directory);
}
