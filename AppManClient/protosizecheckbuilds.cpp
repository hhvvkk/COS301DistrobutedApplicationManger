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
    QList<Build *> builds = management->getAllBuilds();
    while(buildIterator < builds.size()){
        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
        appendJSONValue(jsonMessage, "subHandler", "BuildMD5", true);
        QString buildMD5Value = management->getBuildMD5(builds.at(buildIterator));
        appendJSONValue(jsonMessage, "md5Sum", buildMD5Value, true);
        QString BuildIDString = QString::number(builds.at(buildIterator)->getBuildID());
        appendJSONValue(jsonMessage, "BuildID", BuildIDString, false);
        endJSONMessage(jsonMessage);
        masterSocket->write(jsonMessage.toUtf8().data());
        masterSocket->flush();

        buildIterator++;//go to next build
    }

    //finally to indicate that it is done
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSendBuild", true);
    appendJSONValue(jsonMessage, "subHandler", "SizeCheckAllBuildsDone", false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toUtf8().data());
    masterSocket->flush();

    if(buildIterator >= management->getBuildCount()){
        buildIterator = 0;//reset the iterator
        return;//Done
    }

}

void ProtoSizeCheckBuilds::SizeCheckABuild(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket){

    QString BuildID = jsonObject.value("BuildID").toString();

    Build *theBuild = management->getBuildByID(BuildID.toInt());

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSizeCheckBuilds", true);
    appendJSONValue(jsonMessage, "subHandler", "BuildMD5", true);
    QString BuildIDString = QString::number(theBuild->getBuildID());
    appendJSONValue(jsonMessage, "BuildID", BuildIDString, true);

    //generate the md5 value in a separate thread
    QFuture<QString> future = QtConcurrent::run(management, &Management::getBuildMD5, theBuild);

    QString md5ValueGenerated = future.result();

    appendJSONValue(jsonMessage, "md5Sum", md5ValueGenerated, true);
    appendJSONValue(jsonMessage, "OneBuildOnly", "true", false);
    endJSONMessage(jsonMessage);

    masterSocket->write(jsonMessage.toUtf8().data());
    masterSocket->flush();
}
