#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonDisconnect->setVisible(false);
    //connect(ui->pushButton, SIGNAL(clicked()),this, SLOT(connectClick()));
    management = new Management();
}

MainWindow::~MainWindow()
{
    if(management)
        management->deleteLater();
}

void MainWindow::connectClick()
{
    //needed for parsing
    bool ok;

    int port = -1;

    port = ui->lineEditPort->text().toInt(&ok);

    qDebug()<<"port = "<<port;
    if(!ok || port <= 1000 || port >= 65000){
        QMessageBox *msb = new QMessageBox();
        msb->setText("invalid port number assigned");
        msb->show();
        return;
    }
    ui->groupBoxServerDetails->setTitle("Connected to ");
    ui->lineEditIPAddress->setReadOnly(true);
    ui->lineEditPort->setReadOnly(true);
    ui->pushButtonConnect->setVisible(false);
    ui->pushButtonDisconnect->setVisible(true);
    management->connectToServer(ui->lineEditIPAddress->text(), port);
}

void MainWindow::disconnectClick(){
    ui->groupBoxServerDetails->setTitle("Server Details");
    ui->lineEditIPAddress->setReadOnly(false);
    ui->lineEditPort->setReadOnly(false);
    ui->pushButtonConnect->setVisible(true);
    ui->pushButtonDisconnect->setVisible(false);
    management->disconnectFromServer();
}
