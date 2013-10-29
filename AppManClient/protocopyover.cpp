#include "protocopyover.h"
#include "Build.h"
#include "xmlWriter.h"
#include "management.h"

ProtoCopyOver::ProtoCopyOver(QObject *parent)
    :Protocol(parent)
{
}

void ProtoCopyOver::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket) {
    if(!jsonObject.value("subHandler").toString().compare("CopyBuildOver"))
        CopyBuildOver(jsonObject, management, masterSocket);
}

void ProtoCopyOver::CopyBuildOver(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){

    QString BuildID = jsonObject.value("BuildID").toString();

    bool ok;
    int id = BuildID.toInt(&ok);

    if(!ok)
        return;

    QString buildName = jsonObject.value("buildName").toString();

    xmlWriter xWrite;
    Build *buildToAdd = new Build(id, buildName, "","");
    management->createBuildDirectory(buildToAdd);
    xWrite.receiveBuild(QString::number(buildToAdd->getBuildID()),buildToAdd->getBuildName(),buildToAdd->getBuildDescription(),buildToAdd->getBuildDirectory());
    xWrite.createXMLFile();
    management->addBuild(buildToAdd);

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoCopyOver",true);
    appendJSONValue(jsonMessage,"subHandler","GotABuild",true);
    appendJSONValue(jsonMessage, "BuildID", BuildID,false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toUtf8().data());
    masterSocket->flush();//write all that should be written
}
