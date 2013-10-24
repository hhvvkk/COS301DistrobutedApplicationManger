#include "protoapplist.h"
#include "management.h"

ProtoAppList::ProtoAppList(QObject *parent) : Protocol(parent)
{
    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Connection");
    //send in the default values in case it does not exist...
    QString loadedPort = setting.value("machineID", -1).toString();
    bool validID;
    setting.endGroup();
    int theLoadedId = loadedPort.toInt(&validID);
    machineID = theLoadedId;
}

void ProtoAppList::handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){
    QVariant subHandler = jsonObject.value("subHandler");

    if(!subHandler.toString().compare("requestAppList")){
        requestAppList(slaveSocket, man);
    }
}

void ProtoAppList::requestAppList(QTcpSocket *slaveSocket, Management *man){
    qDebug()<<"requesting";
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoAppList",true);
    appendJSONValue(jsonMessage,"subHandler","appListFollows",true);
    QMap<QString,QString> apps = man->getAppList();
    qDebug()<<apps<<"hi";
    QMapIterator<QString,QString> iter (apps);
    int count = 0;
    while(iter.hasNext()){
        iter.next();
        appendJSONValue(jsonMessage,"AppName"+QString::number(count),iter.key(),true);
        count++;
    }
    appendJSONValue(jsonMessage,"count",QString::number(count),true);
    appendJSONValue(jsonMessage,"MachineID",QString::number(machineID),false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toUtf8().data());
    slaveSocket->flush();
    qDebug()<<jsonMessage;
}
