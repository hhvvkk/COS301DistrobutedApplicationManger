#include "protocolhandler.h"
#include "management.h"

#include "Slave.h"


ProtocolHandler::ProtocolHandler( Management *man, QObject *parent) :
    QObject(parent),
    management(man)
{
    slaveCurrentBuilds = new ProtoSlaveCurrentBuilds(this);
    copyOver = new ProtoCopyOver(this);
    connect = new ProtoConnect(this);
    sizeCheckBuilds = new ProtoSizeCheckBuilds(this);
    getSysInfo = new ProtoGetSysInfo(this);
    sendBuild = new ProtoSendBuild(this);
    firstTalk = true;
}

void ProtocolHandler::handle(QString data){
    QStringList protocolRequests;


    //firstly strip the slashes to reveal all the requests sent through(in case of multiple requests)
    while(data.contains("|")){
        int indexOfLine = -1;
        int startRequestIndex = 0;
        for( ; startRequestIndex < data.length(); startRequestIndex++){
            //a request begins, find the index of vertical line
            if(data.at(startRequestIndex) != '|'){
                indexOfLine = data.indexOf("|", startRequestIndex);
                break;//stop finding next vertical lines
            }
        }

        if(indexOfLine == -1)
            break;//stop if the indexOfLine is not set

        QString newRequest = data.mid(startRequestIndex, (indexOfLine - startRequestIndex));

        protocolRequests.append(newRequest);

        //remove the current request + the lines
        data.remove(0, (indexOfLine + startRequestIndex));
    }


    ///////////HANDLE ALL THE REQUESTS THAT IS SENT THROUGH///////////////
    /////////////E.g. It could be multiple requests///////////////////
    for(int i = 0; i < protocolRequests.length(); i++){
        requestHandler(protocolRequests.at(i));
    }

}

void ProtocolHandler::requestHandler(QString data){
    if(firstTalk == true){
        connect->handle(data, management, slaveSocket);
        firstTalk = false;
        return;
    }

    if(!data.compare("RecheckDone"))
        slaveCurrentBuilds->handle(data, management, slaveSocket);
    else if(!data.compare("RecheckBuilds")){
        slaveCurrentBuilds->handle(data, management, slaveSocket);
    }
    else if(data.contains("Rechecker:#")){
        slaveCurrentBuilds->handle(data, management, slaveSocket);
    }

    if(data.contains("BuildMD5:#"))
        sizeCheckBuilds->handle(data, management, slaveSocket);

    if(data.contains("GotABuild:#"))
        copyOver->handle(data, management, slaveSocket);

    if(data.contains("DetailedSysInfoFollows:#"))
        getSysInfo->handle(data, management, slaveSocket);

    if(data.contains("MinimalSysInfoFollows:#"))
        getSysInfo->handle(data, management, slaveSocket);

    if(!data.compare("SizeCheckAllBuildsDone")){
        sendBuild->handle(data, management, slaveSocket);
    }
}


void ProtocolHandler::setMachine(Machine *m){
    machine = m;
}

Machine* ProtocolHandler::getMachine(){
    return machine;
}


void ProtocolHandler::setSocket(QTcpSocket *socket){
    slaveSocket = socket;
}

void ProtocolHandler::disconnectMachine(){
    ProtoConnect *connectProtocol = dynamic_cast<ProtoConnect*>(connect);
    connectProtocol->disconnectMachine(machine, management);
}

void ProtocolHandler::copyBuildOver(int buildId, QString buildName){
    ProtoCopyOver *copyOverProtocol = dynamic_cast<ProtoCopyOver*>(copyOver);
    copyOverProtocol->copyBuildOver(buildId, buildName, slaveSocket);
}

void ProtocolHandler::getDetStats(){
    ProtoGetSysInfo * sysProto = dynamic_cast<ProtoGetSysInfo*>(getSysInfo);
    sysProto->getDetailed(slaveSocket);
}

void ProtocolHandler::getMinStats(){
    ProtoGetSysInfo * sysProto = dynamic_cast<ProtoGetSysInfo*>(getSysInfo);
    sysProto->getMinimal(slaveSocket);
}
