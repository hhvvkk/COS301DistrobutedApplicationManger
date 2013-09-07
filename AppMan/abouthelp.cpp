#include "abouthelp.h"
#include "ui_abouthelp.h"

AboutHelp::AboutHelp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutHelp)
{
    ui->setupUi(this);
}

AboutHelp::~AboutHelp()
{
    delete ui;
}
