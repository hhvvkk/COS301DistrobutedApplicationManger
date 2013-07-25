#include "Management.h"
#include "Machine.h"
#include "Master.h"
#include "Slave.h"
#include "Build.h"

#include <string>

using namespace std;

Management::Management(){
	
}

Management::~Management(){
	delete[] allBuilds;
	delete[] allMachines;
}