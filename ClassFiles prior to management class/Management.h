#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <string>
#include "Machine.h"
#include "Master.h"
#include "Slave.h"
#include "Build.h"

using namespace std;

class Management{
	private:
		Build * allBuilds;
		Machine * allMachines;
	public:
		Management();
		~Management();
		void addMachine(Master masterToAdd);
		void addMachine(Slave slaveToAdd);
		void addBuild(Build buildToAdd);
};

#endif // MANAGEMENT_H