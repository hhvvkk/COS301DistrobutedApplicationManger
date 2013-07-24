#ifndef MACHINE_H
#define MACHINE_H

#include <string>

using namespace std;

class Machine{
	protected:
		int machineID;
		string machineIP;
		string machineStatus;
	public:
		Machine(int id, string ip, string status);
		~Machine();
		Machine();
	
	
		void setMachine(int id, string ip, string status);
		int getMachineID() {return machineID;};
		string getMachineIP() {return machineIP;};
		string getMachineStatus() {return machineStatus;};
		void setMachineID(int id);
		void setMachineIP(string ip);
		void setMachineStatus(string status);
		void printMachine();
};

#endif // MACHINE_H