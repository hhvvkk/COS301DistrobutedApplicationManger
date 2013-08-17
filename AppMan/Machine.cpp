#include "Machine.h"

Machine::Machine(int id, QString ip){
    if(id <= 0){
		return;
	}
	else{
		machineID = id;
        machineIP = ip;
	}
}

Machine::~Machine(){
	
}

Machine::Machine(){
    machineID = 0;
}

void Machine::setMachine(int id, QString ip){
    if(id <= 0){
		return;
	}
    else if(ip.length()!=15){
		return;
	}
	else{
		machineID = id;
        machineIP = ip;
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

void Machine::setServerThread(ServerThread *sVThread){
    serverThread = sVThread;
}
