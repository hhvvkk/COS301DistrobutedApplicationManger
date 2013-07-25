#ifndef Master_H
#define Master_H

#include "Machine.h"
#include "Build.h"
#include "Slave.h"
#include <string>

using namespace std;

class Master : public Machine{
	private:
		Build * masterBuilds;
		int buildCount;
		Slave * masterSlaves;
		int slaveCount;
	public:
		Master(int id, string ip, string status);
		~Master();
		Master();
	
	
		void deleteBuild(string name);
		void deleteBuild(int id);
		void addBuild(Build buildToAdd);
		void printBuilds();
	
		void deleteSlave(string ip);
		void deleteSlave(int id);
		void addSlave(Slave slaveToAdd);
		void printSlaves();

};

#endif // Master_H