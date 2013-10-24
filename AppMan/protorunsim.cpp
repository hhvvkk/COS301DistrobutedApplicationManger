#include "protorunsim.h"

ProtoRunSim::ProtoRunSim(QObject *parent)
    :Protocol(parent)
{
}

void ProtoRunSim::handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("SimInfoFollows"))
        simRunning(jsonObject, man);
}

void ProtoRunSim::runSim(QTcpSocket *slaveSocket, QString build, QString args){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoRunSim",true);
    appendJSONValue(jsonMessage,"subHandler","runSim",true);
    appendJSONValue(jsonMessage,"buildID",build,true);
    appendJSONValue(jsonMessage,"args",args,false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
    qDebug()<<"sent json message\t"<<jsonMessage;
}

void ProtoRunSim::simRunning(QVariantMap jsonObject, Management *management){

}
