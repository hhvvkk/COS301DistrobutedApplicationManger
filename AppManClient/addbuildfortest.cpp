#include "addbuildfortest.h"
#include "ui_addbuildfortest.h"

addBuildForTest::addBuildForTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBuildForTest)
{
    ui->setupUi(this);
    this->setWindowTitle("Add Build");
    QPixmap pic(":/images/images/ALogoClient.png");
    this->setWindowIcon(QIcon(pic));
    this->ui->pushButton_2->setFocus();
}

addBuildForTest::~addBuildForTest()
{
    delete ui;
}

void addBuildForTest::on_pushButton_clicked()
{
    QString direc = ui->lineEdit_4->text();
    //Check to see if directory actually exist...
    if(!QDir(direc).exists()){
        showError("Invalid directory path","error");
        return;
    }
    QString num = ui->lineEdit->text();
    QString name = ui->lineEdit_2->text();
    QString descrip = ui->lineEdit_3->text();

    if(num.compare("") == 0|| name.compare("") == 0 || descrip.compare("") == 0){
        showError("All fields are required","error");
        return;
    }

    if(!checkBuildID(num)){
        showError("The Build Number cannot be a duplicate entry","error");
        return;
    }

    Build b(num.toInt(),name,descrip,direc);
    showError("Build added successfully","info");
    emit initiateAddBuild(b);
    this->close();
}

bool addBuildForTest::checkBuildID(QString buildNum){
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

void addBuildForTest::showError(QString errorMessage, QString info){
    QMessageBox *msb = new QMessageBox();
    if(info.compare("error") == 0){
        msb->setIcon(QMessageBox::Critical);
    }
    if(info.compare("inform") == 0){
        msb->setIcon(QMessageBox::Information);
    }
    QPixmap pic(":/images/images/ALogoClient.png");
    //msb->setIconPixmap(pic.scaled(50,50,Qt::IgnoreAspectRatio,Qt::FastTransformation));
    msb->setWindowIcon(QIcon(pic));
    msb->setText(errorMessage);
    msb->show();
}

void addBuildForTest::on_pushButton_2_clicked()
{
    QString directory = "";
    if(ui->lineEdit_4->text().compare("")){
        directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),ui->lineEdit_4->text(),QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    }
    else{
        directory = QFileDialog::getExistingDirectory(this, tr("Open Directory"),"",QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
    }

    if(directory.compare(""))
        ui->lineEdit_4->setText(directory);
}
