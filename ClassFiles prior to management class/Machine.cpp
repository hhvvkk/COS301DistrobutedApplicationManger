#include "Machine.h"
#include <iostream>
#include <string>

using namespace std;
/*
		int machineID
		string machineIP;
		string machineStatus;
*/
Machine::Machine(int id, string ip, string status){
	if(id <= 0){
		cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		machineID = id;
		machineIP = ip;
		machineStatus = status;
		cout<<"booo"<<endl;
	}
}
Machine::~Machine(){
	
}
Machine::Machine(){
	machineID = 0;
}

void Machine::setMachine(int id, string ip, string status){
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

void Machine::printMachine(){
	if(this->machineID == 0 || this->machineIP=="" || this->machineStatus=="")
	{
		cout<<"Cannot output an incomplete machine"<<endl;
	}
	else
	{
		cout<<"Machine ID: "<<machineID<<"\t Machine IP: "<<machineIP<<"\t Machine Status: "<<machineStatus<<endl;
	}
}

