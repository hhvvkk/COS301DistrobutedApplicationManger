#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonDisconnect->setVisible(false);
    //connect(ui->pushButton, SIGNAL(clicked()),this, SLOT(connectClick()));
    management = new Management();
    management->addMyBuilds();
    ui->label_BuildCount->setText(QString::number(management->getBuildCount()));


    /*
     *Create the tray(BEGIN)
     **/
    trayIcon = new QSystemTrayIcon(QIcon(":/images/images/ALogoClient.png"), this);
    QAction *quitAction = new QAction( "Exit", trayIcon );
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quitTheApplication()));

    QAction *hideAction = new QAction( "Show/Hide", trayIcon );
    connect(hideAction, SIGNAL(triggered()), this, SLOT(showOrHideTrayClick()));

    QMenu *trayMenu = new QMenu;
    trayMenu->addAction(hideAction);
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();
    //setWindowFlags(windowFlags() | Qt::Tool);
    /*
     *Create the tray(END)
     **/
}


void MainWindow::quitTheApplication(){
    //will be connected to the QApplicaiton and will quit when emitting this
    emit quitApplication();
}

MainWindow::~MainWindow()
{
    if(management)
        management->deleteLater();
}

void MainWindow::showOrHideTrayClick(){
    if(!this->isVisible()){
        show();
        raise();
        setFocus();
    }
    else{
        hide();
    }
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
    ui->label_BuildCount->setText(QString::number(management->getBuildCount()));
}

void MainWindow::disconnectClick(){
    ui->groupBoxServerDetails->setTitle("Server Details");
    ui->lineEditIPAddress->setReadOnly(false);
    ui->lineEditPort->setReadOnly(false);
    ui->pushButtonConnect->setVisible(true);
    ui->pushButtonDisconnect->setVisible(false);
    management->disconnectFromServer();
    ui->label_BuildCount->setText(QString::number(management->getBuildCount()));
}

void MainWindow::on_pushButtonViewBuilds_clicked()
{
    viewBuilds *vb = new viewBuilds();
    xmlReader p = management->getxRead();
    vb->getManager(management);
    vb->getReader(p);
    vb->show();
    ui->label_BuildCount->setText(QString::number(management->getBuildCount()));
}

void MainWindow::initiateAddBuild(Build myBuild){
    Build myAltered = management->createBuildDirectory(myBuild);
    management->addBuild(myAltered);
    xmlWriter xWrite;
    xWrite.receiveBuild(QString::number(myAltered.getBuildID()),myAltered.getBuildName(),myAltered.getBuildDescription(),myAltered.getBuildDirectory());
    xWrite.CreateXMLFile();
    ui->label_BuildCount->setText(QString::number(management->getBuildCount()));
    qDebug()<<"Build added";
}

void MainWindow::on_pushButton_clicked()
{
    addBuildForTest * testBuildAdd = new addBuildForTest();
    connect(testBuildAdd,SIGNAL(initiateAddBuild(Build)),this,SLOT(initiateAddBuild(Build)));
    testBuildAdd->show();
}
