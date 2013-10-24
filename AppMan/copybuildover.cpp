#include "copybuildover.h"
#include "ui_copybuildover.h"

CopyBuildOver::CopyBuildOver(Management *man, QStringList suggestName, QStringList suggestIP, QString buildName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CopyBuildOver),
    management(man)
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

    int machineId = -1;

    for(int i = 0; i < management->getMachineCount(); i++){
        Machine *machine = management->getMachineAt(i);
        if(machine != 0){
            if(!machine->getMachineIP().compare(ui->lineEditMachineIP->text()))
                machineId = machine->getMachineID();
        }
    }

    if(machineId <= -1)
        return;

    emit copyBuildOver(machineId, ui->lineEditBuildName->text());
    this->close();
}

void CopyBuildOver::buildMoreInformationClick(){
    bool buildExist = management->buildExistWithName(ui->lineEditBuildName->text());
    if(buildExist == false){
        showMessage("The build you are enquiring about does not exist", "error");
        return;
    }

    Build *b = management->getBuildByName(ui->lineEditBuildName->text());

    if(b ==0)
        return;

    QString information = "Build Information:\n+ BuildName: "+ b->getBuildName();
    information = information + "\n+ Build ID: " + QString::number(b->getBuildID());
    information = information + "\n+ Build Description: "+ b->getBuildDescription();
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
