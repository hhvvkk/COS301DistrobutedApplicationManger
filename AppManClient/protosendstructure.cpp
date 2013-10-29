#include "protosendstructure.h"
#include "Build.h"
#include "management.h"
#include <QFuture>
#include <QtConcurrentRun>

ProtoSendStructure::ProtoSendStructure(QObject *parent)
    :Protocol(parent)
{
}

ProtoSendStructure::~ProtoSendStructure(){

}


void ProtoSendStructure::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){
    if(!jsonObject.value("subHandler").toString().compare("InitiateGetStructure"))
        InitiateGetStructure(jsonObject, management, masterSocket);
    else if(!jsonObject.value("subHandler").toString().compare("DuplicateStructure"))
        DuplicateStructure(jsonObject, management);
}


void ProtoSendStructure::InitiateGetStructure(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    QString buildID = jsonObject.value("buildID").toString();

    bool ok = false;

    buildID.toInt(&ok);

    if(!ok)
        return;

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSendStructure", true);
    appendJSONValue(jsonMessage, "subHandler", "BuildStructureRequest", true);
    appendJSONValue(jsonMessage, "buildID", buildID, false);
    endJSONMessage(jsonMessage);

    slaveSocket->write(jsonMessage.toUtf8().data());
    slaveSocket->flush();
}

void ProtoSendStructure::DuplicateStructure(QVariantMap jsonObject, Management *management){

    QString buildID = jsonObject.value("buildID").toString();

    bool ok = false;

    int intBuildID = buildID.toInt(&ok);

    if(!ok)
        return;

    QStringList directoriesList;

    QVariantMap mapOfDirs = jsonObject.value("structure").toMap();
    QList<QString> keys = mapOfDirs.keys();

    for(int i = 0; i < keys.size(); i++){
        directoriesList.append(mapOfDirs.value(keys.at(i)).toString());
    }

    QtConcurrent::run(management, &Management::duplicateDirectoryStructure, directoriesList, intBuildID);

}
