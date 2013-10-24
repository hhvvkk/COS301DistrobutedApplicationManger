#include "moreinfo.h"
#include "ui_moreinfo.h"
#include <QCloseEvent>

moreInfo::moreInfo(Machine* m, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::moreInfo)
{    
    machine = m;
    ui->setupUi(this);
    QString detStats = machine->getDetStatsString();

    QStringList noHash = detStats.split("#");
    QStringList* allInfo = parseDetailed(noHash);
    setNetw(allInfo[0]);
    setCPU(allInfo[1]);
    setRAM(allInfo[2]);
    setHDD(allInfo[3]);
    setProcesses(allInfo[4]);

    machine->getDetStats();

    update = new QTimer(this);
    update->setInterval(10000);
    connect(update, SIGNAL(timeout()), this, SLOT(reshow()));
    update->start();
}

moreInfo::~moreInfo()
{
    delete ui;    
    update->deleteLater();

}

QStringList * moreInfo::parseDetailed(QStringList input){
    QStringList * parsed = new QStringList[5];
    for(int i = 0; i < input.length(); i++){
        if(i < 6){
            parsed[0]<<input.at(i);
        }
        else if(i < 8){
            parsed[1]<<input.at(i);
        }
        else{
            if(input.at(i).contains("Drive Label")){
                parsed[3]<<input.at(i);
            }
            else if(input.at(i).contains("Amount")){
                parsed[1]<<input.at(i);
            }
            else if(input.at(i).contains("RAM")){
                parsed[2]<<input.at(i);
            }
            else if(input.at(i).contains("CPU")){
                parsed[2]<<input.at(i);
            }
            else{
                parsed[4]<<input.at(i);
            }
        }
    }
    return parsed;
}

void moreInfo::setRAM(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(2);
    QStringList headers;
    headers.append("Property");
    headers.append("Value");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(0));
        item->setText(1,stringyListy.at(1));
        widg->addTopLevelItem(item);
    }
    ui->vlRAM->addWidget(widg);
}

void moreInfo::setCPU(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(2);
    QStringList headers;
    headers.append("Property");
    headers.append("Value");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(0));
        item->setText(1,stringyListy.at(1));
        widg->addTopLevelItem(item);
    }
    ui->vlCPU->addWidget(widg);
}

void moreInfo::setHDD(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(3);
    QStringList headers;
    headers.append("Drive Label");
    headers.append("Total Capacity");
    headers.append("Used Space");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(1));
        item->setText(1,stringyListy.at(3));
        item->setText(2,stringyListy.at(5));
        widg->addTopLevelItem(item);
    }
    ui->vlHDD->addWidget(widg);
}

void moreInfo::setNetw(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(2);
    QStringList headers;
    headers.append("Property");
    headers.append("Value");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(0));
        item->setText(1,stringyListy.at(1));
        widg->addTopLevelItem(item);
    }
    ui->vlNetStat->addWidget(widg);
}

void moreInfo::setProcesses(QStringList data){
    QString * strings = new QString[data.length()];
    for(int i = 0; i < data.length(); i++){
        strings[i] = data.at(i);
    }
    QTreeWidget * widg = new QTreeWidget();
    widg->setColumnCount(2);
    QStringList headers;
    headers.append("Process ID");
    headers.append("Process Name");
    widg->setHeaderLabels(headers);
    QTreeWidgetItem *item;
    QStringList stringyListy;
    for(int i = 0; i < data.length(); i++){
        stringyListy = strings[i].split(",");
        item = new QTreeWidgetItem();
        item->setText(0,stringyListy.at(0));
        item->setText(1,stringyListy.at(1));
        widg->addTopLevelItem(item);
    }
    ui->vlProcesses->addWidget(widg);
}

void moreInfo::closeEvent(QCloseEvent *event){    
    update->stop();
}

void moreInfo::reshow(){    
    delete ui;
    ui = new Ui::moreInfo;
    ui->setupUi(this);
    QString detStats = machine->getDetStatsString();
    QStringList noHash = detStats.split("#");
    QStringList* allInfo = parseDetailed(noHash);
    setNetw(allInfo[0]);
    setCPU(allInfo[1]);
    setRAM(allInfo[2]);
    setHDD(allInfo[3]);
    setProcesses(allInfo[4]);
    machine->getDetStats();
}
