#include <iostream>
#include <string>
#include <vector>

#include "Machine.h"
#include "Master.h"
#include "Slave.h"

using namespace std;

int main(){
		cout<<endl<<"Making a vector<Machine*> and adding a Master and 2 slave objects and calling Machine::printMachine()"<<endl;
		vector<Machine*> vecMachines;
		Master master(1,"123.456.789.255","online",0);
		Slave slaveBoy(2,"123.555.789.255","online",1);
		vecMachines.push_back(new Master(master));
		vecMachines.push_back(new Slave(slaveBoy));
		vecMachines.push_back(new Slave(3,"255.255.255.255","online",1));
		for( int i = 0; i < vecMachines.size(); i++ ) {
			vecMachines[i]->printMachine();
		}
		cout<<"------------------------"<<endl;
		
		cout<<endl<<"Traversing through vector<Machine*> and adding the Slaves to the Master using Master::addSlave(Machine slaveToAdd) then calling Master::printSlaves()"<<endl;
		cout<<"------------------------"<<endl;
		for( int i = 0; i < vecMachines.size(); i++ ) {
			if((vecMachines[i]->getMachineType()) == 0){
				for( int j = 0; j < vecMachines.size(); j++ ) {
					if((vecMachines[j]->getMachineType()) == 1){
						vecMachines[i]->addSlave(*vecMachines[j]);
					}
				}
				cout<<"------------------------"<<endl;
				vecMachines[i]->printSlaves();
			}
		}
		cout<<"------------------------"<<endl;
		
		
		for( vector<Machine*>::iterator i = vecMachines.begin(); i != vecMachines.end(); ++i )
		{
			delete *i;
		}
		vecMachines.clear();
	return 0;
}