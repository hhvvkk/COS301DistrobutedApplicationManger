#include "Machine.h"

Machine::Machine(int id, QString ip, bool online_, bool type){
    if(id <= 0){
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
		return;
	}
    else if(ip.length()!=15){
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
		return;
	}
	else{
		machineID = id;
	}
}

void Machine::setMachineIP(QString ip){
    if(ip.length()!=15){
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


