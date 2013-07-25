#include "Machine.h"
#include "Master.h"
#include "Slave.h"
#include <iostream>
#include <string>

using namespace std;

int main(){
	cout<<endl<<"Making a Master object and calling Machine::printMachine()"<<endl;
	Master correct(1,"123.456.789.255","online");
	correct.printMachine();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Making a Build object and calling Build::printBuild()"<<endl;
	Build hello(1,"Correct","Correct Values");
	hello.printBuild();
	cout<<"------------------------"<<endl;
	
	Build build2(3,"ID3","To delete");
	
	Build build3(5,"ID5","To delete");
	
	cout<<endl<<"Adding the Build object to the Master object and calling Master::printBuilds()"<<endl;
	correct.addBuild(hello);
	correct.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Adding two more Build objects to the Master object and calling Master::printBuilds()"<<endl;
	correct.addBuild(build2);
	correct.addBuild(build3);
	correct.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Deleting a Build object by name from the Master object and calling Master::printBuilds()"<<endl;
	correct.deleteBuild("ID3");
	correct.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Deleting a Build object by ID from the Master object and calling Master::printBuilds()"<<endl;
	correct.deleteBuild(1);
	correct.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Deleting a Build object by ID that doesn't exist from the Master object and calling Master::printBuilds()"<<endl;
	correct.deleteBuild(1);
	correct.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Deleting a Build object by name that doesn't exist from the Master object and calling Master::printBuilds()"<<endl;
	correct.deleteBuild("ID3");
	correct.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Making a Slave object and calling Machine::printMachine()"<<endl;
	Master slaveBoy(1,"123.456.789.255","online");
	slaveBoy.printMachine();
	cout<<"------------------------"<<endl;
	
	Build build4(2,"ID2","To delete");
	
	Build build5(4,"ID4","To delete");
	
	cout<<endl<<"Adding two Build objects to the Slave object and calling Master::printBuilds()"<<endl;
	slaveBoy.addBuild(build2);
	slaveBoy.addBuild(build3);
	slaveBoy.addBuild(build4);
	slaveBoy.addBuild(build5);
	slaveBoy.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Deleting a Build object by name from the Slave object and calling Slave::printBuilds()"<<endl;
	slaveBoy.deleteBuild("ID3");
	slaveBoy.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Deleting a Build object by ID from the Slave object and calling Slave::printBuilds()"<<endl;
	slaveBoy.deleteBuild(2);
	slaveBoy.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Deleting a Build object by ID that doesn't exist from the Slave object and calling Slave::printBuilds()"<<endl;
	slaveBoy.deleteBuild(2);
	slaveBoy.printBuilds();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Deleting a Build object by name that doesn't exist from the Slave object and calling Slave::printBuilds()"<<endl;
	slaveBoy.deleteBuild("ID3");
	slaveBoy.printBuilds();
	cout<<"------------------------"<<endl;
	/*
	Machine empty;
	empty.printMachine();
	
	empty.setMachine(2,"987.654.321.255","offiline");
	empty.printMachine();
	
	Machine incorrect(-2,"255.255.255.255","Busy");
	
	Machine incomplete;
	incomplete.printMachine();
	incomplete.setMachineID(3);
	incomplete.printMachine();
	incomplete.setMachineIP("137.215.255.255");
	incomplete.printMachine();
	incomplete.setMachineStatus("Busy");
	incomplete.printMachine();
	*/
	cout<<endl<<"Successful test"<<endl;
	return 0;
}