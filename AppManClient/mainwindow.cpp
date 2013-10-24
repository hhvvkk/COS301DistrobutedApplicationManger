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

    connect(management, SIGNAL(signalConnected(bool)), this, SLOT(signalConnected(bool)));

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

    //LOAD connection settings from previous connect

    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Connection");

    //default settings values
    QVariant port;
    QVariant ipAddress;
    port.setValue(4450);
    QString defaultIpAddress = "127.0.0.1";
    ipAddress.setValue(defaultIpAddress);

    //send in the default values in case it does not exist...
    QString loadedPort = setting.value("port", port).toString();
    QString loadedIpAddress = setting.value("ipAddress", ipAddress).toString();

    setting.endGroup();

    bool validatedIp = validateIpAddress(loadedIpAddress);

    if(!validatedIp){
        showMessage("The ip address that was loaded is invalid, reverting to default(127.0.0.1)");
    }else{
        ui->lineEditIPAddress->setText(loadedIpAddress);
    }


    bool validPort;
    int aPort = loadedPort.toInt(&validPort);

    if(!validPort){
        showMessage("The loaded port a valid number(1024-65000), reverting to default(4450)", "error");
    }else{
        if(aPort >= 1024 && aPort <= 65000)
            ui->lineEditPort->setText(loadedPort);
        else{
            showMessage("The loaded port was not within range(1024-65000), reverting to default(4450)", "error");
            ui->lineEditPort->setText("4450");
        }
    }
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


    if(!ok){
        showMessage("The port is not a valid number between 1024 and 65000", "error");
        return;
    }
    if(port <= 1024 || port >= 65000){
        showMessage("The port is not a valid number between 1024 and 65000", "error");
        return;
    }

    bool validIp = validateIpAddress(ui->lineEditIPAddress->text());
    if(!validIp){
        //SHOW AN ERROR
        showMessage("The ip address is not a valid ip address", "error");
        return;
    }

    //else it is valid
    //...and continue

    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Connection");

    setting.setValue("port", port);
    setting.setValue("ipAddress", ui->lineEditIPAddress->text());

    setting.endGroup();
    management->connectToServer(ui->lineEditIPAddress->text(), port);
}


void MainWindow::disconnectClick(){
    management->disconnectFromServer();
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
    xWrite.createXMLFile();
    ui->label_BuildCount->setText(QString::number(management->getBuildCount()));
}

void MainWindow::on_pushButton_clicked()
{
    addBuildForTest * testBuildAdd = new addBuildForTest();
    connect(testBuildAdd,SIGNAL(initiateAddBuild(Build)),this,SLOT(initiateAddBuild(Build)));
    testBuildAdd->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    sysInfo s;
    qDebug()<<s.getDetailedStats();
    qDebug()<<s.getMinimalStats();
}


void MainWindow::signalConnected(bool connectionValue){
    if(connectionValue == true){
        //means connected Thus display the connected values
        showConnectedWidgets();
    }
    else{
        //means connected Thus display the connected values
        showDisconnectedWidgets();
    }
}

void MainWindow::showConnectedWidgets(){
    ui->groupBoxServerDetails->setTitle("Connected to ");
    ui->lineEditIPAddress->setReadOnly(true);
    ui->lineEditPort->setReadOnly(true);
    ui->pushButtonConnect->setVisible(false);
    ui->pushButtonDisconnect->setVisible(true);
    ui->label_BuildCount->setText(QString::number(management->getBuildCount()));
}

void MainWindow::showDisconnectedWidgets(){
    ui->groupBoxServerDetails->setTitle("Server Details");
    ui->lineEditIPAddress->setReadOnly(false);
    ui->lineEditPort->setReadOnly(false);
    ui->pushButtonConnect->setVisible(true);
    ui->pushButtonDisconnect->setVisible(false);
    ui->label_BuildCount->setText(QString::number(management->getBuildCount()));
}

void MainWindow::showMessage(QString message, QString flag){

    QMessageBox *msb = new QMessageBox();

    if(!flag.compare("error"))
         msb->setIcon(QMessageBox::Critical);
    else
         msb->setIcon(QMessageBox::Information);

    QPixmap pic(":/images/images/ALogoClient.png");
    msb->setWindowIcon(QIcon(pic));
    msb->setText(message);
    msb->show();
}

bool MainWindow::validateIpAddress(const QString &ipAddress){
    QHostAddress address(ipAddress);
    if (QAbstractSocket::IPv4Protocol == address.protocol()){
          return true;
    }

    return false;
}

void MainWindow::on_pushButton_3_clicked()
{
    AddApp * appAdder = new AddApp();
    appAdder->show();
}
