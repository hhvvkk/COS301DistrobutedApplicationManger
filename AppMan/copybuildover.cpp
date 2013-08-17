#include "copybuildover.h"
#include "ui_copybuildover.h"

CopyBuildOver::CopyBuildOver(Management *man, QStringList suggestName, QStringList suggestIP, QString buildName, QWidget *parent) :
    management(man),
    QWidget(parent),
    ui(new Ui::CopyBuildOver)
{
    ui->setupUi(this);

    if(buildName != 0)
        ui->lineEditBuildName->setText(buildName);
    nameSuggestor = new QCompleter(suggestName, this);
    ui->lineEditBuildName->setCompleter(nameSuggestor);

    ipSuggestor = new QCompleter(suggestIP,this);
    ui->lineEditMachineIP->setCompleter(ipSuggestor);
}

CopyBuildOver::~CopyBuildOver()
{
    delete ui;
}

void CopyBuildOver::cancelClick(){
    this->close();
}

void CopyBuildOver::okClick(){
    if(!validateInput())
        return;
    emit copyBuildOver(ui->lineEditMachineIP->text(), ui->lineEditBuildName->text());
    this->close();
}

void CopyBuildOver::buildMoreInformationClick(){
    bool buildExist = management->buildExistWithName(ui->lineEditBuildName->text());
    if(buildExist == false){
        showMessage("The build you are enquiring about does not exist", "error");
        return;
    }

    Build b = management->getBuildByName(ui->lineEditBuildName->text());
    QString information = "Build Information:\n+ BuildName: "+b.getBuildName();
    information = information + "\n+ Build Number: " + QString::number(b.getBuildID());
    information = information + "\n+ Build Description: "+ b.getBuildDescription();
    showMessage(information, "inform");

}

bool CopyBuildOver::validateInput(){
    if(!management->buildExistWithName(ui->lineEditBuildName->text())){
        showMessage("Invalid build name entered", "error");
        return false;
    }

    if(!management->machineExistWithIp(ui->lineEditMachineIP->text())){
        showMessage("Invalid IP adress entered or that Machine is not online", "error");
        return false;
    }
    return true;
}

void CopyBuildOver::showMessage(QString errorMessage, QString info){
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
