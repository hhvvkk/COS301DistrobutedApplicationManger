#include "Build.h"

Build::Build(int id, QString name, QString descript, QString direc){
    if(id <= -1){
		return;
	}
	else{
		buildID = id;
		buildName = name;
		buildDescription = descript;
        buildDirectory = direc;
	}
    //set the issame for the slaveBuilds
    isSame = true;
}
Build::~Build(){
	buildID = -1;
	buildName = "";
	buildDescription = "";
    buildDirectory = "";
}
Build::Build(){
	buildID = 0;
}
void Build::setBuild(int id, QString name, QString descript, QString direc){
    if(id <= -1){
		return;
	}
	else{
		buildID = id;
		buildName = name;
		buildDescription = descript;
        buildDirectory = direc;
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
void Build::setBuildDirectory(QString direc){
    buildDirectory = direc;
}

void Build::setIsSame(bool same){
   isSame = same;
}

bool Build::getIsSame(){
    return isSame;
}

