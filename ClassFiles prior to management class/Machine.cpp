#include "Machine.h"
#include <iostream>
#include <string>

using namespace std;
/*
		int machineID
		string machineIP;
		string machineStatus;
*/
Machine::Machine(int id, string ip, string status, bool type){
	if(id <= 0){
		cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		machineID = id;
		machineIP = ip;
		machineStatus = status;
		machineType = type;
	}
}
Machine::~Machine(){
	
}
Machine::Machine(){
	machineID = 0;
	machineType = 1;
}

void Machine::setMachine(int id, string ip, string status, bool type){
	if(id <= 0){
		cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else if(ip.length()!=15){
		cout<<"The IP is invalid"<<endl;
		return;
	}
	else{
		machineID = id;
		machineIP = ip;
		machineStatus = status;
		machineType = type;
	}
}
void Machine::setMachineID(int id){
	if(id <= 0){
		cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		machineID = id;
	}
}
void Machine::setMachineIP(string ip){
	if(ip.length()!=15){
		cout<<"The IP is invalid"<<endl;
		return;
	}
	else{
		machineIP = ip;
	}
}
void Machine::setMachineStatus(string status){
	machineStatus = status;
}
void Machine::setMachineType(bool type){
	machineType = type;
}

void Machine::printMachine(){
	if(this->machineID == 0 || this->machineIP=="" || this->machineStatus=="")
	{
		cout<<"Cannot output an incomplete machine"<<endl;
	}
	else
	{
		string theType = "";
		if(machineType == 0){
			theType = "Master";
		}
		else{
			theType = "Slave";
		}
		cout<<"Machine ID: "<<machineID<<"\tMachine IP: "<<machineIP<<"\tMachine Status: "<<machineStatus<<"\t\tMachine Type: "<<theType<<endl;
	}
}

