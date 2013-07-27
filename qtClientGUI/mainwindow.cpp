#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString masterIP = "123.123.231.132";
    goToNext(masterIP);
}

void MainWindow::goToNext(QString ip){
    myDialog = new Dialog();
    myDialog->setIP(ip);
    myDialog->show();
}
