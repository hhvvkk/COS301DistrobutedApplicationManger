#include "Machine.h"
//#include <iostream>
#include <QString>

using namespace std;
/*
		int machineID
                QString machineIP;
                QString machineStatus;
*/
Machine::Machine(int id, QString ip, bool online_, bool type){
	if(id <= 0){
                //cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		machineID = id;
		machineIP = ip;
                online = online_;
		machineType = type;
	}
}

Machine::~Machine(){
	
}

Machine::Machine(){
	machineID = 0;
	machineType = 1;
}

void Machine::setMachine(int id, QString ip, bool onL, bool type){
	if(id <= 0){
                //cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else if(ip.length()!=15){
                //cout<<"The IP is invalid"<<endl;
		return;
	}
	else{
		machineID = id;
		machineIP = ip;
                online = onL;
		machineType = type;
	}
}

void Machine::setMachineID(int id){
	if(id <= 0){
                //cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		machineID = id;
	}
}

void Machine::setMachineIP(QString ip){
	if(ip.length()!=15){
                //cout<<"The IP is invalid"<<endl;
		return;
	}
	else{
		machineIP = ip;
	}
}

void Machine::setMachineOnline(bool onL){
        online = onL;
}

void Machine::setMachineType(bool type){
	machineType = type;
}

void Machine::printMachine(){
        if(this->machineID == 0 || this->machineIP=="")
	{
                //cout<<"Cannot output an incomplete machine"<<endl;
	}
	else
	{
                QString theType = "";
		if(machineType == 0){
			theType = "Master";
		}
		else{
			theType = "Slave";
		}
                //cout<<"Machine ID: "<<machineID<<"\tMachine IP: "<<machineIP<<"\tMachine Status: "<<machineStatus<<"\t\tMachine Type: "<<theType<<endl;
	}
}

