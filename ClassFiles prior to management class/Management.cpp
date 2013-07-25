#include "Management.h"
#include "Machine.h"
#include "Master.h"
#include "Slave.h"
#include "Build.h"

#include <string>
#include <iostream>

using namespace std;

Management::Management(){
	buildCount = 0;
	machineCount = 0;
}

Management::~Management(){
	cout<<"Starting to delete from management.cpp"<<endl;
	if(buildCount != 0)
	{	
		delete[] allBuilds;
	}
	if(machineCount != 0){
		for( vector<Machine*>::iterator i = allMachines.begin(); i != allMachines.end(); ++i )
		{
			delete *i;
		}
		allMachines.clear();
	}
	cout<<"Delete success at management.cpp"<<endl;
}

void Management::addMachine(Master masterToAdd){
	allMachines.push_back(new Master(masterToAdd));
	machineCount++;
}

void Management::addMachine(Slave slaveToAdd){
	allMachines.push_back(new Slave(slaveToAdd));
	machineCount++;
}

void Management::addBuild(Build buildToAdd){
	if(buildCount != 0){
		Build temp[buildCount];
		for(int i = 0; i < buildCount; i++){
			temp[i] = allBuilds[i];
		}
		allBuilds = new Build[buildCount+1]; 
		for(int j = 0; j < buildCount; j++){
			allBuilds[j] = temp[j];
		}
		allBuilds[buildCount] = buildToAdd;
	}
	else{
		allBuilds = new Build[buildCount+1]; 
		allBuilds[buildCount] = buildToAdd;
	}
	buildCount++;
}
