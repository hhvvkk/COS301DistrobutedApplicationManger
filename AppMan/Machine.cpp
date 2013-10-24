#include "Machine.h"
#include <QHostAddress>

Machine::Machine(int id, QString ip)
    :machineID(id),
    machineIP(ip){
    minStats="0%#0%#0.0MB#0.0KB" ;
    detStats="Data Transmitted,0MB#Data Received,0KB#Packets Transmitted,0#Packets Received,0#Errors Transmitting,0#Errors Receiving,0#Number of Processors,0#Operating System,None#Drive Label,C:,Total Capacity,0GB,Used Space,0GB#Total RAM,0GB#Used RAM,0GB#% RAM in use,0#% CPU in use,0#Amount of Processes active,0#0,Please wait... Gathering information." ;
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

 QString Machine::getMinStatsString(){
     return minStats;
 }

 QString Machine::getDetStatsString(){
     return detStats;
 }

 void Machine::setMinStatsString(QString data){
     minStats = data;
 }
 void Machine::setDetStatsString(QString data){
     detStats = data;
 }
