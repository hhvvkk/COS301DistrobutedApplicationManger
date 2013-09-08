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

    QString buildNo = jsonObject.value("buildNo").toString();
    QString buildName = jsonObject.value("buildName").toString();
    Build newBuild = Build(buildNo.toInt() , buildName,"", "");

    xmlWriter xWrite;
    Build buildToAdd = management->createBuildDirectory(newBuild);
    xWrite.receiveBuild(QString::number(buildToAdd.getBuildID()),buildToAdd.getBuildName(),buildToAdd.getBuildDescription(),buildToAdd.getBuildDirectory());
    xWrite.CreateXMLFile();
    management->addBuild(buildToAdd);

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler","ProtoCopyOver",true);
    appendJSONValue(jsonMessage,"subHandler","GotABuild",true);
    appendJSONValue(jsonMessage, "buildNo", buildNo,false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toAscii().data());
    masterSocket->flush();//write all that should be written
}
