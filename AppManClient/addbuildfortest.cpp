#include "addbuildfortest.h"
#include "ui_addbuildfortest.h"

addBuildForTest::addBuildForTest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addBuildForTest)
{
    ui->setupUi(this);
}

addBuildForTest::~addBuildForTest()
{
    delete ui;
}

void addBuildForTest::on_pushButton_clicked()
{
    QString num = ui->lineEdit->text();
    QString name = ui->lineEdit_2->text();
    QString descrip = ui->lineEdit_3->text();
    QString direc = ui->lineEdit_4->text();
    Build b(num.toInt(),name,descrip,direc);
    emit initiateAddBuild(b);
    this->close();
}
