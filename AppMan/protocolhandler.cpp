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
    updateBuildInfo = new ProtoUpdateBuildInfo(this);
    updateMachineInfo = new ProtoUpdateMachineInfo(this);
    deleteBuild = new ProtoDeleteBuild(this);
    aRunSim = new ProtoRunSim(this);
    appList = new ProtoAppList(this);
    sendStructure = new ProtoSendStructure(this);
    firstTalk = true;
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
    deleteBuild->deleteLater();
    aRunSim->deleteLater();
    appList->deleteLater();
    sendStructure->deleteLater();
}

void ProtocolHandler::handle(QString data){
    if(slaveSocket == 0){
        return;
    }

    QFuture <QStringList>future = QtConcurrent::run(this, &ProtocolHandler::splitRequests, data);

    QStringList protocolRequests = future.result();

    if(protocolRequests.isEmpty())
        return;//no requests found

    ///////////HANDLE ALL THE REQUESTS THAT IS SENT THROUGH///////////////
    /////////////E.g. It could be multiple requests///////////////////
    for(int i = 0; i < protocolRequests.length(); i++){
        requestHandler(protocolRequests.at(i));
    }

}

QStringList ProtocolHandler::splitRequests(QString data){
    QStringList requests;
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

        requests.append(newRequest);

        //remove the current request + the lines
        data.remove(0, (indexOfLine + startRequestIndex));
    }

    return requests;
}

void ProtocolHandler::requestHandler(QString data){
    JSON *instance = &JSON::instance();

    //go and parse the json to an object concurrently...
    QFuture <QVariantMap>future = QtConcurrent::run(instance, &JSON::parse, data);

    const QVariantMap jsonObject = future.result();//finally retrieve the jsonObject after it has been parsed
    QVariant handler = jsonObject.value("handler");

    if(firstTalk == true){
        connect->handle(jsonObject, management, slaveSocket);
        firstTalk = false;
        return;
    }
    if(!handler.toString().compare("QVariant(, )")){
        qDebug()<< "invalid JSON String::"<<data;
        return;
    }

    if(!handler.toString().compare("ProtoConnect"))
        connect->handle(jsonObject, management, slaveSocket);
    else

    if(!handler.toString().compare("ProtoSlaveCurrentBuilds"))
        slaveCurrentBuilds->handle(jsonObject, management, slaveSocket);
    else

    if(!handler.toString().compare("ProtoGetSysInfo"))
        getSysInfo->handle(jsonObject, management, slaveSocket);
    else
    if(!handler.toString().compare("ProtoSizeCheckBuilds"))
       sizeCheckBuilds->handle(jsonObject, management, slaveSocket);
    else

    if(!handler.toString().compare("ProtoSendBuild"))
        sendBuild->handle(jsonObject, management, slaveSocket);
    else

    if(!handler.toString().compare("ProtoCopyOver"))
        copyOver->handle(jsonObject, management, slaveSocket);
    else

    if(!handler.toString().compare("ProtoUpdateBuildInfo"))
        updateBuildInfo->handle(jsonObject, management, slaveSocket);
    else

    if(!handler.toString().compare("ProtoUpdateMachineInfo"))
        updateMachineInfo->handle(jsonObject, management, slaveSocket);
    else


    if(!handler.toString().compare("ProtoDeleteBuild"))
        deleteBuild->handle(jsonObject, management, slaveSocket);

    else

    if(!handler.toString().compare("ProtoRunSim"))
        aRunSim->handle(jsonObject, management, slaveSocket);

    else

    if(!handler.toString().compare("ProtoAppList"))
        appList->handle(jsonObject, management, slaveSocket);
    else

    if(!handler.toString().compare("ProtoSendStructure"))
        sendStructure->handle(jsonObject, management, slaveSocket);

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
    ProtoConnect *connectProtocolClass = dynamic_cast<ProtoConnect*>(connect);
    if(connectProtocolClass == 0)
        return;
    connectProtocolClass->disconnectMachine(machine, management);
}

void ProtocolHandler::copyBuildOver(int buildId, QString buildName){
    ProtoCopyOver *copyOverProtocolClass = dynamic_cast<ProtoCopyOver*>(copyOver);
    if(copyOverProtocolClass == 0)
        return;
    copyOverProtocolClass->copyBuildOver(buildId, buildName, slaveSocket);
}

void ProtocolHandler::getDetStats(){
    ProtoGetSysInfo * sysProtoClass = dynamic_cast<ProtoGetSysInfo*>(getSysInfo);
    if(sysProtoClass == 0)
        return;
    sysProtoClass->getDetailed(slaveSocket);
}

void ProtocolHandler::getMinStats(){
    ProtoGetSysInfo * sysProtoClass = dynamic_cast<ProtoGetSysInfo*>(getSysInfo);
    if(sysProtoClass == 0)
        return;
    sysProtoClass->getMinimal(slaveSocket);
}

void ProtocolHandler::recheckAllSizes(){
    ProtoSizeCheckBuilds *sizeCheckerClass = dynamic_cast<ProtoSizeCheckBuilds*>(sizeCheckBuilds);
    if(sizeCheckerClass == 0)
        return;
    sizeCheckerClass->invokeSizeCheckAll(slaveSocket);
}

void ProtocolHandler::slaveABuildSizeDone(){
    ProtoSendBuild * theSendBuild = dynamic_cast<ProtoSendBuild*>(sendBuild);
    if(theSendBuild == 0)
        return;
    theSendBuild->sizeCheckCertainBuildDone(/*buildID, machine, management,*/ slaveSocket);
}

void ProtocolHandler::updateBuildName(int buildID, QString newBuildName){
    ProtoUpdateBuildInfo *theUpdater = dynamic_cast<ProtoUpdateBuildInfo*>(updateBuildInfo);
    if(theUpdater == 0)
        return;
    theUpdater->updateName(buildID, newBuildName, slaveSocket);
}


void ProtocolHandler::updateUniqueID(int uniqueID){
    ProtoUpdateMachineInfo *theUpdater = dynamic_cast<ProtoUpdateMachineInfo*>(updateMachineInfo);
    if(theUpdater == 0)
        return;
    theUpdater->updateUniqueID(uniqueID, slaveSocket);
}


void ProtocolHandler::deleteBuildFromSlave(int BuildID){
    ProtoDeleteBuild *theDeleter = dynamic_cast<ProtoDeleteBuild*>(deleteBuild);
    if(theDeleter == 0)
        return;
    theDeleter->deleteBuildFromSlave(slaveSocket, BuildID);
}

void ProtocolHandler::runSim(QString build, QString args){
    ProtoRunSim * simProtoClass = dynamic_cast<ProtoRunSim*>(aRunSim);
    if(simProtoClass == 0)
        return;
    simProtoClass->runSim(slaveSocket,build,args);
}

