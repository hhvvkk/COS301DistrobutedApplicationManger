#include "addbuild.h"
#include "ui_addbuild.h"

AddBuild::AddBuild(Management *man, QString directory, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddBuild),
    management(man)
{
    ui->setupUi(this);
    ui->lineEditDirectory->setText(directory);
}

AddBuild::~AddBuild()
{
    delete ui;
}

void AddBuild::cancelClick(){
    this->close();
}

void AddBuild::okClick(){
    //management->addBuild(); ..?

    //NOTA::AS jy kan sit by management se addBuild() ook i signal en connect dit dan kan die
    //   ...  mainform die build wat added is display word
    //NOTA 2: vir voorbeelde check mainform.cpp se constctor van die MasterBuild class
    //      en signals + addMachine(....) function in die management.cpp
}

void AddBuild::chooseClick(){

}


