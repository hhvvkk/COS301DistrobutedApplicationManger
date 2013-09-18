#include "protoconnect.h"
#include "management.h"
#include "socketclient.h"

ProtoConnect::ProtoConnect(QObject *parent)
    :Protocol(parent)
{
    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Connection");

    //default settings values
    QVariant defaultMachineId;
    defaultMachineId.setValue(-1);

    //send in the default values in case it does not exist...
    QString loadedPort = setting.value("machineID", defaultMachineId).toString();

    setting.endGroup();

    bool validID;
    int theLoadedId = loadedPort.toInt(&validID);

    if(!validID){
        machineID = -1;
    }else{
        if(theLoadedId >= 0)
            machineID = theLoadedId;
        else{
            machineID = -1;
        }
    }
}

void ProtoConnect::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){

    QVariant subHandler = jsonObject.value("subHandler");

    if(!subHandler.toString().compare("Hello AppManClient")){
        //means a connection was established
        initiateSlaveCurrentBuilds(masterSocket);
        management->setConnected(true);
        return;
    }
}

void ProtoConnect::disconnectFromMaster(Management *management, QTcpSocket *socket){
    management->setConnected(false);
    if(socket == 0){
        return;
    }
    socket->disconnectFromHost();
}

void ProtoConnect::initiateSlaveCurrentBuilds(QTcpSocket *masterSocket){
    //to get an acknowledgement that the builds can be rechecked
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSlaveCurrentBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "RecheckBuilds", false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toAscii().data());
    masterSocket->flush();
}

void ProtoConnect::connectToMaster(QString ipAddress, int serverPort, ProtocolHandler * protocolHandler){

    QTcpSocket *socket = new QTcpSocket(this);

    //making sure it is disconnectedclose();

    socket->connectToHost(ipAddress, serverPort);

    //wait for one second for connection
    if(!socket->waitForConnected(1000)){
        //error
        qDebug()<<"Error when connecting";
        return;
    }

    SocketClient *socketClient = new SocketClient(protocolHandler, socket);

    //connect(socket, SIGNAL(connected()), socketClient, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), socketClient, SLOT(readyRead()));
    connect(socket, SIGNAL(disconnected()), socketClient, SLOT(disconnected()));

    //write to the server to connect
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoConnect", true);
    appendJSONValue(jsonMessage, "subHandler", "Hello AppMan", true);
    appendJSONValue(jsonMessage, "machineID", QString::number(machineID), false);
    endJSONMessage(jsonMessage);

    socket->write(jsonMessage.toAscii().data());
    socket->flush();

    //finally set the socket so that the network can use it
    protocolHandler->setSocket(socket);
}

void ProtoConnect::SetMachineID(int newMachineID){

    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Connection");

    setting.setValue("machineID", newMachineID);

    setting.endGroup();
}
