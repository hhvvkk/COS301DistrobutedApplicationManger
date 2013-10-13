#include "protocol.h"

Protocol::Protocol(QObject *parent) :
    QObject(parent)
{
}

Protocol::~Protocol(){

}

QString Protocol::startJSONMessage(){
    return QString("{");
}

void Protocol::appendJSONValue(QString &currentString, QString newKey, QString newValue, bool addComma){
    QString appendThis = "\""+newKey+"\" : \""+newValue+"\"";
    if(addComma)
        appendThis.append(",");
    currentString.append(appendThis);
}



void Protocol::endJSONMessage(QString &currentString){
    currentString.prepend("||");
    currentString.append("}");
    currentString.append("||");
}

void Protocol::sendJSONMessage(QTcpSocket *slaveSocket, QString jsonMessage){
    if(slaveSocket == 0){
        return;
    }

    try{
        slaveSocket->write(jsonMessage.toAscii().data());
        slaveSocket->flush();
    }catch(...){
        //if it fails to write and throws an exception somehow, you catch the exception
    }
}
