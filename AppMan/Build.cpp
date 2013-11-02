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
    isSame = false;
    buildMD5 = 0;
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

void Build::generateBuildMD5(){
    if(buildMD5 != 0){
        if(!buildMD5->isOld()){
            buildMD5->setIsOld(true);
        }
        buildMD5->tryDelete();
    }

    BuildMD5 *newBuildMD5 = new BuildMD5(buildDirectory, 5);

    QFuture <void>future = QtConcurrent::run(newBuildMD5, &BuildMD5::generate);
    future.waitForFinished();//wait untill the md5 class has been created

    buildMD5 = newBuildMD5;
}

BuildMD5 *Build::getBuildMD5Class(){
    //lock the generate function such that it does not have to be generated twice or more by two calling this
    lock.lock();
    if(buildMD5 == 0){
        //generate it
        generateBuildMD5();
    }
    else if(buildMD5->isOld()){
        generateBuildMD5();
    }

    lock.unlock();
    return buildMD5;
}

void Build::setBuildMD5Expired(){
    lock.lock();
    if(buildMD5 != 0)
        buildMD5->setIsOld();
    buildMD5->tryDelete();
    buildMD5 = 0;
    lock.unlock();
}
