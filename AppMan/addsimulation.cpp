#include "addsimulation.h"
#include "ui_addsimulation.h"

AddSimulation::AddSimulation(QWidget *parent, Management * man) :
    QDialog(parent),
    ui(new Ui::AddSimulation)
{
    management = man;
    ui->setupUi(this);
    hideStuff();
    setup();
}

AddSimulation::~AddSimulation()
{
    delete ui;
}

void AddSimulation::hideStuff(){
    ui->slaveCheck01->hide();
    ui->slaveCheck02->hide();
    ui->slaveCheck03->hide();
    ui->slaveCheck04->hide();
    ui->slaveCheck05->hide();

    ui->buildComboBox01->hide();
    ui->buildComboBox02->hide();
    ui->buildComboBox03->hide();
    ui->buildComboBox04->hide();
    ui->buildComboBox05->hide();

    ui->argEdit01->hide();
    ui->argEdit02->hide();
    ui->argEdit03->hide();
    ui->argEdit04->hide();
    ui->argEdit05->hide();
}

void AddSimulation::on_pushButton_clicked()
{
    this->close();
}

void AddSimulation::on_pushButton_2_clicked()
{
    QString simName = ui->lineEdit->text();
    if(simName.compare("") == 0){
        QMessageBox *msb = new QMessageBox();
        msb->setIcon(QMessageBox::Critical);
        QPixmap pic(":/images/images/ALogo.png");
        msb->setWindowIcon(QIcon(pic));
        msb->setText("Simulation name must be entered");
        msb->show();
    }else{
        QStringList * slaves = new QStringList();
        QStringList * builds = new QStringList();
        QStringList * args = new QStringList();
        Simulation * sim = new Simulation(simName);
        int slaveCnt = 0;
        int cnt = management->getMachineCount();
        for(int i = 0; i < cnt; i++){
            if(i == 0){
                if(ui->slaveCheck01->isChecked()){
                    slaves->append(ui->slaveCheck01->text());
                    builds->append(ui->buildComboBox01->currentText());
                    args->append(ui->argEdit01->text());
                    slaveCnt++;
                }
            }else if(i == 1){
                if(ui->slaveCheck02->isChecked()){
                    slaves->append(ui->slaveCheck02->text());
                    builds->append(ui->buildComboBox02->currentText());
                    args->append(ui->argEdit02->text());
                    slaveCnt++;
                }
            }else if(i == 2){
                if(ui->slaveCheck03->isChecked()){
                    slaves->append(ui->slaveCheck03->text());
                    builds->append(ui->buildComboBox03->currentText());
                    args->append(ui->argEdit03->text());
                    slaveCnt++;
                }
            }else if(i == 3){
                if(ui->slaveCheck04->isChecked()){
                    slaves->append(ui->slaveCheck04->text());
                    builds->append(ui->buildComboBox04->currentText());
                    args->append(ui->argEdit04->text());
                    slaveCnt++;
                }
            }else if(i == 4){
                if(ui->slaveCheck05->isChecked()){
                    slaves->append(ui->slaveCheck05->text());
                    builds->append(ui->buildComboBox05->currentText());
                    args->append(ui->argEdit05->text());
                    slaveCnt++;
                }
            }
        }

        if(slaveCnt > 0){
            sim->addSlave(slaves,builds,args);
            emit initiateAddSimulation(sim);
            this->close();
        }
        else{
            QMessageBox *msb = new QMessageBox();
            msb->setIcon(QMessageBox::Critical);
            QPixmap pic(":/images/images/ALogo.png");
            msb->setWindowIcon(QIcon(pic));
            msb->setText("Slaves must be added");
            msb->show();
        }
    }

}

void AddSimulation::setup(){
    QList<Build*> allBuilds = management->getAllBuilds();
    for(int i = 0; i < management->getBuildCount(); i++){
        QString item = "";
        item += QString::number(allBuilds[i]->getBuildID());
        item += "-";
        item += allBuilds[i]->getBuildName();
        ui->buildComboBox01->addItem(item);
        ui->buildComboBox02->addItem(item);
        ui->buildComboBox03->addItem(item);
        ui->buildComboBox04->addItem(item);
        ui->buildComboBox05->addItem(item);
    }
    allMachines = management->getAllMachines();
    int cnt = management->getMachineCount();
    for(int i = 0; i < cnt; i++){
        QString ckbLabel = QString::number(allMachines[i]->getMachineID()) + "-" + allMachines[i]->getMachineIP();
        if(i==0){
            ui->slaveCheck01->setText(ckbLabel);
            ui->slaveCheck01->show();
            ui->argEdit01->show();
            ui->buildComboBox01->show();
        }
        else if(i==1){
            ui->slaveCheck02->setText(ckbLabel);
            ui->slaveCheck02->show();
            ui->argEdit02->show();
            ui->buildComboBox02->show();
        }
        else if(i==2){
            ui->slaveCheck03->setText(ckbLabel);
            ui->slaveCheck03->show();
            ui->argEdit03->show();
            ui->buildComboBox03->show();
        }
        else if(i==3){
            ui->slaveCheck04->setText(ckbLabel);
            ui->slaveCheck04->show();
            ui->argEdit04->show();
            ui->buildComboBox04->show();
        }
        else if(i==4){
            ui->slaveCheck05->setText(ckbLabel);
            ui->slaveCheck05->show();
            ui->argEdit05->show();
            ui->buildComboBox05->show();
        }
    }
}
