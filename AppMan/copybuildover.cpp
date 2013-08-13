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
    qDebug()<<"ok clicked";
}

void CopyBuildOver::buildMoreInformationClick(){
    bool buildExist = management->buildExistWithName(ui->lineEditBuildName->text());
    if(buildExist == false){
        showError("The build you are enquiring about does not exist");
        return;
    }


}

void CopyBuildOver::showError(QString error){
    QMessageBox *errorMessage = new QMessageBox();
    errorMessage->setText(error);
    errorMessage->show();
}
