#include "Machine.h"
#include "Slave.h"
#include <iostream>
#include <string>

using namespace std;
/*
		int machineID
		string machineIP;
		string machineStatus;
*/
Slave::Slave(int id, string ip, string status){
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
Slave::~Slave(){
	cout<<"Starting to delete"<<endl;
	//for(int i = 0; i<= buildCount; i++){
		delete[] slaveBuilds/*[i]*/;
	//}
	cout<<"Delete success"<<endl;
}
Slave::Slave(){
	machineID = 0;
	buildCount = 0;
}

void Slave::addBuild(Build buildToAdd){
	//cout<<"here"<<endl;
	if(buildCount != 0){
		//cout<<"already have"<<endl;
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
		//cout<<"dont have"<<endl;
		slaveBuilds = new Build[buildCount+1];
		slaveBuilds[buildCount] = buildToAdd;
	}
	//masterBuilds[buildCount] = buildToAdd;
	buildCount++;
	//cout<<"leaving"<<endl;
}

void Slave::printBuilds(){
	cout<<"Build ID: "<<"\tBuild name: "<<"\tBuild Description: "<<endl;
	for(int i = 0; i< buildCount; i++){
		cout<<slaveBuilds[i].getBuildID()<<"\t\t"<<slaveBuilds[i].getBuildName()<<"\t\t"<<slaveBuilds[i].getBuildDescription()<<endl;
	}
}

void Slave::deleteBuild(string name){
	int delIndex = -1;
	for(int i = 0; i< buildCount; i++){
		if(slaveBuilds[i].getBuildName().compare(name) == 0){
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
	else{
		cout<<"Build not found"<<endl;
	}
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
	else{
		cout<<"Build not found"<<endl;
	}
}

