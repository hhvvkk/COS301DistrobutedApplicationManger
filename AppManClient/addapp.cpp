#include "addapp.h"
#include "ui_addapp.h"
#include "management.h"

AddApp::AddApp(QWidget *parent, Management *man) :
    QDialog(parent),
    ui(new Ui::AddApp)
{
    ui->setupUi(this);
    manage =man;
}

AddApp::~AddApp()
{
    delete ui;
}

void AddApp::on_pushButton_clicked()
{
    this->close();
}

void AddApp::on_pushButton_3_clicked()
{
    //Choose the directory
    QString filename = QFileDialog::getOpenFileName(this,"Select File");
    ui->lineEdit_2->setText(filename);
}

void AddApp::on_pushButton_2_clicked()
{
    QString appName = ui->lineEdit->text();
    QString fileName = ui->lineEdit_2->text();
    //Send to management and write to xml
    appXMLWriter axw;
    axw.receiveApp(appName,fileName);
    axw.createXMLFile();
    manage->addToAppList(appName,fileName);
    this->close();
}
