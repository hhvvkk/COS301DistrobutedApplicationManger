#ifndef Slave_H
#define Slave_H

#include "Machine.h"
#include "Build.h"
#include <string>

using namespace std;

class Slave : public Machine{
	private:
		Build * slaveBuilds;
		int buildCount;
	public:
		Slave(int id, string ip, string status);
		~Slave();
		Slave();
	
	
		void deleteBuild(string name);
		void deleteBuild(int id);
		void addBuild(Build buildToAdd);
		void printBuilds();

};

#endif // Slave_H