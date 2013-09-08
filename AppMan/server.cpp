#include "server.h"

Server::Server(Management *m,QObject *parent) :
    QTcpServer(parent),
    management(m)
{
    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Connection");

    //default settings values
    QVariant defaultPort;
    defaultPort.setValue(4450);

    //send in the default values in case it does not exist...
    QString loadedPort = setting.value("port", defaultPort).toString();

    setting.endGroup();

    bool validPort;
    int aPort = loadedPort.toInt(&validPort);

    if(!validPort){
        showMessage("The loaded port a valid number(1024-65000), reverting to default(4450)", "error");
        port = 4450;
    }else{
        if(aPort >= 1024 && aPort <= 65000)
            port = aPort;
        else{
            showMessage("The loaded port was not within range(1024-65000), reverting to default(4450)", "error");
            port = 4450;
        }
    }
}

void Server::startServer(){
    if(!this->listen(QHostAddress::Any,port)){
        qDebug() << "Could not start server";
    }
    else{
        qDebug() << "Listening..."<<port;
    }
}


void Server::stopServer(){
    Server::close();
}

void Server::setPort(int newPort){
    port = newPort;
    stopServer();
    startServer();
}

void Server::incomingConnection(int socketID){

    ServerThread *serverThread = new ServerThread(socketID, management, this);

    //connect(serverThread, SIGNAL(finished()), serverThread, SLOT(deleteLater()));

    serverThread->startThread();
}


void Server::showMessage(QString message, QString flag){

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
