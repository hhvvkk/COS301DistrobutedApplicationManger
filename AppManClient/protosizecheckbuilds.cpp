#include "protosizecheckbuilds.h"
#include "management.h"

ProtoSizeCheckBuilds::ProtoSizeCheckBuilds(QObject *parent)
    :Protocol(parent)
{
    buildIterator = 0;
}

void ProtoSizeCheckBuilds::handle(QString data, Management *management, QTcpSocket *masterSocket){
    if(!data.compare("SizeCheckAllBuilds")){
        sendCurrentBuildMD5(management, masterSocket);
    }
}

void ProtoSizeCheckBuilds::sendCurrentBuildMD5(Management *management, QTcpSocket *masterSocket){
    Build * builds = management->getAllBuilds();
    while(buildIterator < management->getBuildCount())
    {
        //Example : ||BuildMD5:#1#d41d8cd98f00b204e9800998ecf8427e||
        QString md5Message = "||BuildMD5:#";
        md5Message = md5Message + QString::number(builds[buildIterator].getBuildID()) + "#"+management->getBuildMD5(builds[buildIterator]);
        md5Message = md5Message + "||";
        masterSocket->write(md5Message.toAscii().data());
        masterSocket->flush();

        buildIterator++;//go to next build
    }

    if(buildIterator >= management->getBuildCount()){
        buildIterator = 0;//reset the iterator
        return;//Done
    }
}
