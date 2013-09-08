#include "protocol.h"

Protocol::Protocol(QObject *parent) :
    QObject(parent)
{
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
