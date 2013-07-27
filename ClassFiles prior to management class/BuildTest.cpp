#include <iostream>
#include <string>

#include "Build.h"

using namespace std;

int main(){
	cout<<endl<<"Making a Build object and calling Build::printBuild()"<<endl;
	Build correct(1,"Correct","Correct Values");
	correct.printBuild();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Making an empty Build object and calling Build::printBuild()"<<endl;
	Build empty;
	empty.printBuild();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Calling Build::setBuild(2,\"Empty\",\"Empty but filled in\"); for the empty Build object and calling Build::printBuild()"<<endl;
	empty.setBuild(2,"Empty","Empty but filled in");
	empty.printBuild();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Making a Build object with a negative ID"<<endl;
	Build incorrect(-2,"Wrong","The ID is negative");
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Making an incomplete Build object and calling Build::printBuild()"<<endl;
	Build incomplete;
	incomplete.printBuild();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Adding just the ID to the incomplete build and calling Build::printBuild()"<<endl;
	incomplete.setBuildID(3);
	incomplete.printBuild();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Now adding the Name to the incomplete build and calling Build::printBuild()"<<endl;
	incomplete.setBuildName("Incomplete Build");
	incomplete.printBuild();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Now adding the Description to the incomplete build and calling Build::printBuild()"<<endl;
	incomplete.setBuildDescription("This build was incomplete but is now filled in");
	incomplete.printBuild();
	cout<<"------------------------"<<endl;
	
	cout<<endl<<"Successful test"<<endl<<endl<<"Destrctors at work:"<<endl<<endl;
	return 0;
}