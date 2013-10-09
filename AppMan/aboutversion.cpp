#include "aboutversion.h"
#include "ui_aboutversion.h"

AboutVersion::AboutVersion(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutVersion)
{
    ui->setupUi(this);
}

AboutVersion::~AboutVersion(){
}
