#include "Slave.h"


Slave::Slave(int id, QString ip)
    :Machine(id, ip){
    buildCount = 0;
}

Slave::~Slave(){
    if(buildCount != 0){
        delete[] slaveBuilds;
    }
    if(protocolHandler != 0)
        protocolHandler->deleteLater();
}

Slave::Slave(){
	machineID = 0;
    buildCount = 0;
}

Slave::Slave(Slave &toCopy)
    :Machine(toCopy.machineID, toCopy.machineIP){
    this->buildCount = toCopy.buildCount;
    this->slaveBuilds = toCopy.slaveBuilds;
}

void Slave::addBuild(Build buildToAdd){
	for(int i = 0; i < buildCount; i++){
        //if any of them are similar, return
            if(buildToAdd.getBuildID() == slaveBuilds[i].getBuildID())
                return;
	}
	if(buildCount != 0){
		Build temp[buildCount];
		for(int i = 0; i < buildCount; i++){
			temp[i] = slaveBuilds[i];
		}
		slaveBuilds = new Build[buildCount+1];
		for(int j = 0; j < buildCount; j++){
			slaveBuilds[j] = temp[j];
		}
		slaveBuilds[buildCount] = buildToAdd;
	}
	else{
		slaveBuilds = new Build[buildCount+1];
		slaveBuilds[buildCount] = buildToAdd;
	}
	buildCount++;
}

void Slave:: deleteBuild(int id){
	
	int delIndex = -1;
	for(int i = 0; i< buildCount; i++){
		if(slaveBuilds[i].getBuildID() == id){
			delIndex = i;
		}
	}
	if(delIndex >= 0){
		Build temp[buildCount-1];
		for(int j = 0; j < buildCount; j++){
			
			if(j < delIndex){
				temp[j] = slaveBuilds[j];
			}
			else if(j > delIndex){
				temp[j-1] = slaveBuilds[j];
			}
		}
		slaveBuilds = new Build[buildCount-1];
		for(int k = 0; k < buildCount-1; k++){
			slaveBuilds[k] = temp[k];
		}
		buildCount --;
	}
}

void Slave::copyBuildOver(Build build){
    protocolHandler->copyBuildOver(build.getBuildID(), build.getBuildName());
}



void Slave::setProtocolHandler(ProtocolHandler *handler){
    protocolHandler = handler;
}



Build *Slave::getBuilds(){
    return slaveBuilds;
}


int Slave::getBuildCount(){
    return buildCount;
}

void Slave::setBuildSame(bool isSame, int buildID){
    Build *builds = getBuilds();
    for(int i = 0; i < getBuildCount(); i++){
        if(buildID == builds[i].getBuildID())
            builds[i].setIsSame(isSame);
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
