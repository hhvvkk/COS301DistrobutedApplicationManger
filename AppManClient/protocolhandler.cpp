#include "protocolhandler.h"
#include "management.h"

ProtocolHandler::ProtocolHandler(Management *man, QObject *parent) :
    QObject(parent),
    management(man)
{
    sizeCheckBuilds = new ProtoSizeCheckBuilds(this);
    slaveCurrentBuilds = new ProtoSlaveCurrentBuilds(this);
    connect = new ProtoConnect(this);
    copyOver = new ProtoCopyOver(this);
    getSysInfo = new ProtoGetSysInfo(this);
    sendBuild = new ProtoSendBuild(this);
    updateBuildInfo = new ProtoUpdateBuildInfo(this);
    updateMachineInfo = new ProtoUpdateMachineInfo(this);
    masterSocket = 0;
}

ProtocolHandler::~ProtocolHandler(){
    slaveCurrentBuilds->deleteLater();
    copyOver->deleteLater();
    connect->deleteLater();
    sizeCheckBuilds->deleteLater();
    getSysInfo->deleteLater();
    sendBuild->deleteLater();
    updateBuildInfo->deleteLater();
    updateMachineInfo->deleteLater();
}

void ProtocolHandler::handle(QString data){

    QStringList protocolRequests;

    if(masterSocket == 0){
        qDebug()<<"masterSocket = 0(handle--protocolHandler)";
        return;
    }
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

    const QVariantMap jsonObject = JSON::instance().parse(data);
    QVariant handler = jsonObject.value("handler");

    if(!handler.toString().compare("QVariant(, )")){
        qDebug()<< "invalid JSON String::"<<data;
        return;
    }

    if(!handler.toString().compare("ProtoConnect"))
        connect->handle(jsonObject, management, masterSocket);
    else

    if(!handler.toString().compare("ProtoSlaveCurrentBuilds"))
        slaveCurrentBuilds->handle(jsonObject, management, masterSocket);
    else

    if(!handler.toString().compare("ProtoGetSysInfo"))
        getSysInfo->handle(jsonObject, management, masterSocket);
    else

    if(!handler.toString().compare("ProtoSizeCheckBuilds"))
        sizeCheckBuilds->handle(jsonObject, management, masterSocket);
    else

    if(!handler.toString().compare("ProtoSendBuild"))
        sendBuild->handle(jsonObject, management, masterSocket);
    else

    if(!handler.toString().compare("ProtoCopyOver"))
        copyOver->handle(jsonObject, management, masterSocket);
    else

    if(!handler.toString().compare("ProtoUpdateBuildInfo"))
        updateBuildInfo->handle(jsonObject, management, masterSocket);
    else

    if(!handler.toString().compare("ProtoUpdateMachineInfo"))
        updateMachineInfo->handle(jsonObject, management, masterSocket);

}

void ProtocolHandler::disconnectFromMaster(){
    ProtoConnect *connectProtocol = dynamic_cast<ProtoConnect*>(connect);
    if(connectProtocol == 0)
        return;
    connectProtocol->disconnectFromMaster(management, masterSocket);
}


void ProtocolHandler::connectToServer(QString IpAddress, int serverPort){
    ProtoConnect *connectProtocol = dynamic_cast<ProtoConnect*>(connect);
    if(connectProtocol == 0)
        return;
    connectProtocol->connectToMaster(IpAddress, serverPort, this);
}

void ProtocolHandler::setSocket(QTcpSocket *newSocket){
    if(newSocket == 0){
        return;
    }
    masterSocket = newSocket;
}
