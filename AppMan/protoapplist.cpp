#include "protoapplist.h"
#include "management.h"
ProtoAppList::ProtoAppList(QObject *parent) : Protocol(parent){

}

void ProtoAppList::handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){
    QVariant subHandler = jsonObject.value("subHandler");
    //
    if(!subHandler.toString().compare("appListFollows")){
        appListFollows(jsonObject,slaveSocket, man);

    }
    if(!subHandler.toString().compare("requestAppList")){
        requestAppList(slaveSocket);
    }
}

void ProtoAppList::requestAppList(QTcpSocket *slaveSocket){
     QString jsonMessage = startJSONMessage();
     appendJSONValue(jsonMessage,"handler","ProtoAppList",true);
     appendJSONValue(jsonMessage,"subHandler","requestAppList",false);
     endJSONMessage(jsonMessage);

     sendJSONMessage(slaveSocket, jsonMessage);

}

void ProtoAppList::appListFollows(QVariantMap jsonObject, QTcpSocket *slaveSocket, Management *man){
    qDebug()<<jsonObject<<"haha";
    int machine = jsonObject.value("MachineID").toString().toInt();
    Machine * mach = man->getMachineById(machine);
    int count = jsonObject.value("count").toString().toInt();
    qDebug()<<QString::number(count);
    for(int it = 0; it < count; it++){
        //qDebug()<<;
        mach->addToAppList(jsonObject.value("AppName"+QString::number(it)).toString());
    }
}
