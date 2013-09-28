#include "protosizecheckbuilds.h"
#include "management.h"

ProtoSizeCheckBuilds::ProtoSizeCheckBuilds(QObject *parent)
    :Protocol(parent)
{
    buildIterator = 0;
}

void ProtoSizeCheckBuilds::handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){
    if(!jsonObject.value("subHandler").toString().compare("SizeCheckAllBuilds")){
        sendCurrentBuildMD5(management, masterSocket);
    }
    if(!jsonObject.value("subHandler").toString().compare("SizeCheckABuild"))
        SizeCheckABuild(jsonObject, management, masterSocket);
}

void ProtoSizeCheckBuilds::sendCurrentBuildMD5(Management *management, QTcpSocket *masterSocket){
    Build * builds = management->getAllBuilds();
    while(buildIterator < management->getBuildCount())
    {
        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
        appendJSONValue(jsonMessage, "subHandler", "BuildMD5", true);
        appendJSONValue(jsonMessage, "md5Sum", management->getBuildMD5(builds[buildIterator]), true);
        QString BuildIDString = QString::number(builds[buildIterator].getBuildID());
        appendJSONValue(jsonMessage, "BuildID", BuildIDString, false);
        endJSONMessage(jsonMessage);

        masterSocket->write(jsonMessage.toAscii().data());
        masterSocket->flush();

        buildIterator++;//go to next build
    }

    //finally to indicate that it is done
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSendBuild", true);
    appendJSONValue(jsonMessage, "subHandler", "SizeCheckAllBuildsDone", false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toAscii().data());
    masterSocket->flush();

    if(buildIterator >= management->getBuildCount()){
        buildIterator = 0;//reset the iterator
        return;//Done
    }
}

void ProtoSizeCheckBuilds::SizeCheckABuild(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){

    QString BuildID = jsonObject.value("BuildID").toString();

    Build theBuild = management->getBuildByID(BuildID.toInt());

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "BuildMD5", true);
    QString BuildIDString = QString::number(theBuild.getBuildID());
    appendJSONValue(jsonMessage, "BuildID", BuildIDString, true);
    appendJSONValue(jsonMessage, "md5Sum", management->getBuildMD5(theBuild), true);
    appendJSONValue(jsonMessage, "OneBuildOnly", "true", false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toAscii().data());
    masterSocket->flush();
}
