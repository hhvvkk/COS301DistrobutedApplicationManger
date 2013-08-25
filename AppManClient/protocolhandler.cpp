#include "protocolhandler.h"
#include "management.h"

ProtocolHandler::ProtocolHandler(Management *man,QObject *parent) :
    QObject(parent),
    management(man)
{
    sizeCheckBuilds = new ProtoSizeCheckBuilds(this);
    slaveCurrentBuilds = new ProtoSlaveCurrentBuilds(this);
    connect = new ProtoConnect(this);
    copyOver = new ProtoCopyOver(this);
    getSysInfo = new ProtoGetSysInfo(this);
}


void ProtocolHandler::handle( QString data, QTcpSocket *masterSocket){

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
        requestHandler(protocolRequests.at(i), masterSocket);
    }


}

void ProtocolHandler::requestHandler(QString data, QTcpSocket *masterSocket){
    if(!data.compare("SizeCheckAllBuilds")){
        //which means execute sizeCheckAllBuilds protocol
        sizeCheckBuilds->handle(data, management, masterSocket);
    }
    if(!data.compare("RecheckCopy")){
        slaveCurrentBuilds->handle(data, management, masterSocket);
    }

    if(!data.compare("Hello AppManClient"))
        connect->handle(data, management, masterSocket);

    if(data.contains("CopyBuildOver:#"))
        copyOver->handle(data, management, masterSocket);

    if(data.contains("SizeCheckABuild:#"))
        sizeCheckBuilds->handle(data, management, masterSocket);

    if(data.compare("GetAllSysInfo"))
        getSysInfo->handle(data, management, masterSocket);
}

void ProtocolHandler::disconnectedFromMaster(){
    ProtoConnect *connectProtocol = dynamic_cast<ProtoConnect*>(connect);
    connectProtocol->disconnectedFromMaster(management);
}
