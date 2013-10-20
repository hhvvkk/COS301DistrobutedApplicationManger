#include "Machine.h"
#include <QHostAddress>

Machine::Machine(int id, QString ip)
    :machineID(id),
    machineIP(ip){

}

Machine::~Machine(){
	
}

Machine::Machine(){
    machineID = 0;
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
    if(!validateIpAddress(ip)){
        return;
    }
    else{
        machineIP = ip;
    }
}

bool Machine::validateIpAddress(const QString &ipAddress){
    QHostAddress address(ipAddress);
    if (QAbstractSocket::IPv4Protocol == address.protocol()){
          return true;
    }

    return false;
}
