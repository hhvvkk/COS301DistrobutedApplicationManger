#include "Machine.h"
#include "Master.h"
#include <iostream>
#include <string>

using namespace std;
/*
		int machineID
		string machineIP;
		string machineStatus;
*/
Master::Master(int id, string ip, string status){
	if(id <= 0){
		cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		machineID = id;
		machineIP = ip;
		machineStatus = status;
		buildCount = 0;
	}
}
Master::~Master(){
	cout<<"Starting to delete"<<endl;
	delete[] masterBuilds;
	//delete[] masterSlaves;
	cout<<"Delete success"<<endl;
}
Master::Master(){
	machineID = 0;
	buildCount = 0;
}

void Master::addSlave(Slave slaveToAdd){
	if(slaveCount != 0){
		Slave temp[slaveCount];
		for(int i = 0; i < slaveCount; i++){
			temp[i] = masterSlaves[i];
		}
		masterSlaves = new Slave[slaveCount+1];
		for(int j = 0; j < slaveCount; j++){
			masterSlaves[j] = temp[j];
		}
		masterSlaves[slaveCount] = slaveToAdd;
	}
	else{
		//cout<<"dont have"<<endl;
		masterSlaves = new Slave[slaveCount+1];
		masterSlaves[slaveCount] = slaveToAdd;
	}
	//masterSlaves[slaveCount] = slaveToAdd;
	slaveCount++;
	//cout<<"leaving"<<endl;
}

void Master::printSlaves(){
	cout<<"Slave ID: "<<"\tSlave IP: "<<"\tSlave Status: "<<endl;
	for(int i = 0; i< slaveCount; i++){
		cout<<masterSlaves[i].getMachineID()<<"\t\t"<<masterSlaves[i].getMachineIP()<<"\t\t"<<masterSlaves[i].getMachineStatus()<<endl;
	}
}

void Master::deleteSlave(string ip){
	int delIndex = -1;
	for(int i = 0; i< slaveCount; i++){
		if(masterSlaves[i].getMachineIP().compare(ip) == 0){
			delIndex = i;
		}
	}
	if(delIndex >= 0){
		Slave temp[slaveCount-1];
		for(int j = 0; j < slaveCount; j++){
			
			if(j < delIndex){
				temp[j] = masterSlaves[j];
			}
			else if(j > delIndex){
				temp[j-1] = masterSlaves[j];
			}
		}
		masterSlaves = new Slave[slaveCount-1];
		for(int k = 0; k < slaveCount-1; k++){
			masterSlaves[k] = temp[k];
		}
		slaveCount --;
	}
	else{
		cout<<"Slave not found"<<endl;
	}
}

void Master:: deleteSlave(int id){
	
	int delIndex = -1;
	for(int i = 0; i< slaveCount; i++){
		if(masterSlaves[i].getMachineID() == id){
			delIndex = i;
		}
	}
	if(delIndex >= 0){
		Slave temp[slaveCount-1];
		for(int j = 0; j < slaveCount; j++){
			
			if(j < delIndex){
				temp[j] = masterSlaves[j];
			}
			else if(j > delIndex){
				temp[j-1] = masterSlaves[j];
			}
		}
		masterSlaves = new Slave[slaveCount-1];
		for(int k = 0; k < slaveCount-1; k++){
			masterSlaves[k] = temp[k];
		}
		slaveCount --;
	}
	else{
		cout<<"Slave not found"<<endl;
	}
}


void Master::addBuild(Build buildToAdd){
	if(buildCount != 0){
		Build temp[buildCount];
		for(int i = 0; i < buildCount; i++){
			temp[i] = masterBuilds[i];
		}
		masterBuilds = new Build[buildCount+1];
		for(int j = 0; j < buildCount; j++){
			masterBuilds[j] = temp[j];
		}
		masterBuilds[buildCount] = buildToAdd;
	}
	else{
		//cout<<"dont have"<<endl;
		masterBuilds = new Build[buildCount+1];
		masterBuilds[buildCount] = buildToAdd;
	}
	//masterBuilds[buildCount] = buildToAdd;
	buildCount++;
	//cout<<"leaving"<<endl;
}

void Master::printBuilds(){
	cout<<"Build ID: "<<"\tBuild Name: "<<"\tBuild Description: "<<endl;
	for(int i = 0; i< buildCount; i++){
		cout<<masterBuilds[i].getBuildID()<<"\t\t"<<masterBuilds[i].getBuildName()<<"\t\t"<<masterBuilds[i].getBuildDescription()<<endl;
	}
}

void Master::deleteBuild(string name){
	int delIndex = -1;
	for(int i = 0; i< buildCount; i++){
		if(masterBuilds[i].getBuildName().compare(name) == 0){
			delIndex = i;
		}
	}
	if(delIndex >= 0){
		Build temp[buildCount-1];
		for(int j = 0; j < buildCount; j++){
			
			if(j < delIndex){
				temp[j] = masterBuilds[j];
			}
			else if(j > delIndex){
				temp[j-1] = masterBuilds[j];
			}
		}
		masterBuilds = new Build[buildCount-1];
		for(int k = 0; k < buildCount-1; k++){
			masterBuilds[k] = temp[k];
		}
		buildCount --;
	}
	else{
		cout<<"Build not found"<<endl;
	}
}

void Master:: deleteBuild(int id){
	
	int delIndex = -1;
	for(int i = 0; i< buildCount; i++){
		if(masterBuilds[i].getBuildID() == id){
			delIndex = i;
		}
	}
	if(delIndex >= 0){
		Build temp[buildCount-1];
		for(int j = 0; j < buildCount; j++){
			
			if(j < delIndex){
				temp[j] = masterBuilds[j];
			}
			else if(j > delIndex){
				temp[j-1] = masterBuilds[j];
			}
		}
		masterBuilds = new Build[buildCount-1];
		for(int k = 0; k < buildCount-1; k++){
			masterBuilds[k] = temp[k];
		}
		buildCount --;
	}
	else{
		cout<<"Build not found"<<endl;
	}
}




