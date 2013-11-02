#include "Slave.h"


Slave::Slave(int id, QString ip)
    :Machine(id, ip){
    buildCount = 0;
}

Slave::~Slave(){
    Build *build;
    while(allBuilds.size() >= 1){
        build = allBuilds.takeAt(0);
        delete build;
    }
    if(protocolHandler != 0)
        protocolHandler->deleteLater();
}

Slave::Slave(){
	machineID = 0;
    buildCount = 0;
}

void Slave::addBuild(Build *buildToAdd){
    allBuilds.append(buildToAdd);
}

void Slave:: deleteBuild(int id){
	
    for(int i = 0; i < allBuilds.size(); i++){
        Build *b = 0;
        if(allBuilds.at(i)->getBuildID() == id){
            b = allBuilds.takeAt(i);
            break;
        }
        if(b != 0)
            delete b;
    }
}

void Slave::copyBuildOver(Build *build){
    protocolHandler->copyBuildOver(build->getBuildID(), build->getBuildName());
}



void Slave::setProtocolHandler(ProtocolHandler *handler){
    protocolHandler = handler;
}


Build* Slave::getBuildAt(int i){
    if(i < 0 || i >= allBuilds.size())
        return 0;
    return allBuilds.at(i);
}


int Slave::getBuildCount(){
    return allBuilds.size();
}

void Slave::setBuildSame(bool isSame, int buildID){
    for(int i = 0; i < getBuildCount(); i++){
        Build *build = allBuilds.at(i);
        if(build == 0)
            continue;

        if(buildID == build->getBuildID())
            build->setIsSame(isSame);
    }
}

void Slave::getMinStats(){
    protocolHandler->getMinStats();
}

void Slave::getDetStats(){
    protocolHandler->getDetStats();
}

void Slave::slaveABuildSizeDone(){
    protocolHandler->slaveABuildSizeDone();
}


void Slave::updateBuildName(int buildID, QString newBuildName){
    protocolHandler->updateBuildName(buildID, newBuildName);
}


void Slave::updateUniqueID(int uniqueID){
    protocolHandler->updateUniqueID(uniqueID);
}

void Slave::deleteBuildNotify(int BuildID){
    protocolHandler->deleteBuildFromSlave(BuildID);
}


void Slave::resynchAll(){
    protocolHandler->recheckAllSizes();
}

void Slave::runSim(QString build, QString args){
    protocolHandler->runSim(build,args);
}

void Slave::addToAppList(QString name){
    appList<<name;
}
