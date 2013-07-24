#include "Build.h"
#include <string>
#include <iostream>

using namespace std;

Build::Build(int id, string name, string descript){
	if(id <= -1){
		cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		buildID = id;
		buildName = name;
		buildDescription = descript;
	}
	
}
Build::~Build(){
	buildID = -1;
	buildName = "";
	buildDescription = "";
}
Build::Build(){
	buildID = 0;
}
void Build::setBuild(int id, string name, string descript){
	if(id <= -1){
		cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		buildID = id;
		buildName = name;
		buildDescription = descript;
	}
}
void Build::setBuildID(int id){
	buildID = id;
}
void Build::setBuildName(string name){
	buildName = name;
}
void Build::setBuildDescription(string descript){
	buildDescription = descript;
}

void Build::printBuild(){
	if(this->buildID == 0 || this->buildName=="" || this->buildDescription=="")
	{
		cout<<"Cannot output an incomplete build"<<endl;
	}
	else
	{
		cout<<"Build ID: "<<buildID<<"\t Build name: "<<buildName<<"\t Build Description: "<<buildDescription<<endl;
	}
}