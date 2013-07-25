#include <iostream>
#include <string>
#include <vector>

#include "Build.h"
#include "Machine.h"
#include "Management.h"
#include "Master.h"
#include "Slave.h"

using namespace std;

int main(){
	Management appManager;
	Master master1(1,"123.123.123.123","Online");
	Slave  slave1(2,"123.123.123.124","Online");
	Build build1(1,"build1","The first program build in the appManager");
	appManager.addMachine(master1);
	appManager.addMachine(slave1);
	appManager.addBuild(build1);
	Build * allBuildsHere = appManager.getAllBuilds();
	vector<Machine*> vec= appManager.getAllMachines();
	vec.front()->addSlave(slave1);
	cout<<"amount in vector"<<appManager.getMachineCount()<<endl;
	
	//*allMachinesHere[0]->addSlave(*allMachinesHere[1]);
	return 0;
}