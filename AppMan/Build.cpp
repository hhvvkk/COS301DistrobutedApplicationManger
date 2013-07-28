#include "Build.h"
#include <QString>
//#include <iostream>

using namespace std;

Build::Build(int id, QString name, QString descript){
	if(id <= -1){
                ////cout<<"Cannot have a negative ID"<<endl;
		return;
	}
	else{
		buildID = id;
		buildName = name;
		buildDescription = descript;
	}
	
}
Build::~Build(){
        //cout<<"Starting to delete from build"<<endl;
	buildID = -1;
	buildName = "";
	buildDescription = "";
        //cout<<"Delete success from build"<<endl;
}
Build::Build(){
	buildID = 0;
}
void Build::setBuild(int id, QString name, QString descript){
	if(id <= -1){
                //cout<<"Cannot have a negative ID"<<endl;
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
void Build::setBuildName(QString name){
	buildName = name;
}
void Build::setBuildDescription(QString descript){
	buildDescription = descript;
}

void Build::printBuild(){
	if(this->buildID == 0 || this->buildName=="" || this->buildDescription=="")
	{
                //cout<<"Cannot output an incomplete build"<<endl;
	}
	else
	{
                //cout<<"Build ID: "<<buildID<<"\t Build name: "<<buildName<<"\t Build Description: "<<buildDescription<<endl;
	}
}
