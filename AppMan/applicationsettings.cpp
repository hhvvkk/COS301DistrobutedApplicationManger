#include "applicationsettings.h"
#include "ui_applicationsettings.h"

ApplicationSettings::ApplicationSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplicationSettings)
{
    ui->setupUi(this);
}

ApplicationSettings::~ApplicationSettings()
{
    delete ui;
}
