#include "management.h"

Management::Management()
{
    network = new Network(this);
    buildCount = 0;

    //if the directory for all builds does not exist, create it
    ///@todo change directory of builds...
    allBuildsDirectory = "builds";
    if(!QDir(allBuildsDirectory).exists())
        QDir().mkdir(allBuildsDirectory);
}


Management::~Management()
{
    delete network;
    if(buildCount != 0)
    {
        delete[] allBuilds;
    }
    //delete xRead;
}

void Management::connectToServer(QString ipAddress, int port){
    network->setConnectionDetails(ipAddress, port);
    network->connecToServer();
}

void Management::disconnectFromServer(){
    network->disconnect();
}

void Management::addBuild(Build buildToAdd){
    //create the directory for that build if it does not exist
    Build myBuild = createBuildDirectory(buildToAdd);


    if(buildCount != 0){
        Build temp[buildCount];
        for(int i = 0; i < buildCount; i++){
            temp[i] = allBuilds[i];
        }
        allBuilds = new Build[buildCount+1];
        for(int j = 0; j < buildCount; j++){
            allBuilds[j] = temp[j];
        }
        allBuilds[buildCount] = myBuild;
    }
    else{
        allBuilds = new Build[buildCount+1];
        allBuilds[buildCount] = myBuild;
    }
    buildCount++;
}

Build Management::createBuildDirectory(Build build){
    QString newBuildDirectory = QString::number(build.getBuildID());
    if(!QDir(allBuildsDirectory+"/"+newBuildDirectory).exists())
    {
        QDir().mkdir(allBuildsDirectory+"/"+newBuildDirectory);
    }
    build.setBuildDirectory(allBuildsDirectory+"/"+newBuildDirectory);
    return build;
}

void Management::addMyBuilds(){
    //xRead = new xmlReader();
    xRead.parseXML();

    QMap<QString,QString> buildsID = xRead.getBuildUniqueID();
    QMap<QString,QString> buildsName = xRead.getBuildName();
    QMap<QString,QString> buildsDesc = xRead.getBuildDescription();
    QMap<QString,QString> buildsDir = xRead.getBuildDirectory();

    QMapIterator<QString, QString> i(buildsID);
    QMapIterator<QString, QString> j(buildsName);
    QMapIterator<QString, QString> k(buildsDesc);
    QMapIterator<QString, QString> l(buildsDir);

    while (i.hasNext() && j.hasNext() && k.hasNext() && l.hasNext())
    {
        i.next(); j.next(); k.next(); l.next();
        Build b(i.value().toInt(),j.value(),k.value(),l.value());
        this->addBuild(b);
    }
}

Build &Management::getBuildByID(int id){
    lock.lock();
    for(int i = 0; i < buildCount; i++){
        if(allBuilds[i].getBuildID() == id){
            lock.unlock();
            return allBuilds[i];
        }
    }
    // Returns a build with id 0 to show it does not exist
    Build b(0,"NULL","NULL","NULL");
    lock.unlock();
    return b;
}

QString Management::getBuildMD5(Build build){
    BuildMD5 md5(build.getBuildDirectory(),5);
    md5.generate();
    return md5.getDirectoryMD5();
}

void Management::setConnected(bool connectionValue){
    emit signalConnected(connectionValue);
}

QString Management::getDetSysInfo(){

    return systeminfo.getDetailedStats();
}

QString Management::getMinSysInfo(){
    return systeminfo.getMinimalStats();
}

void Management::updateBuildName(int buildID, QString newBuildName){
    Build &theBuild = getBuildByID(buildID);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        return;
    }

    theBuild.setBuildName(newBuildName);
    xmlWriter writer;
    writer.updateBuildName(buildID, newBuildName);
}


void Management::updateUniqueID(int uniqueID){
    QSettings setting("settings.ini",QSettings::IniFormat);

    //grouping the settings
    setting.beginGroup("Connection");

    setting.setValue("machineID", uniqueID);

    setting.endGroup();
}

void Management::addToAppList(QString appName, QString appDir){
    appList.insertMulti(appName,appDir);
}

QMap<QString,QString> Management::getAppList(){
    return appList;
}


void Management::deleteBuild(int buildID){

    Build &theBuild = getBuildByID(buildID);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        return;
    }

    QString buildDirectory = theBuild.getBuildDirectory();

    //go and delete build
    //from (1): logical view point
    removeBuildLogically(buildID);

    //from (2): xml
    xmlWriter aWriter;
    aWriter.removeBuild(buildID);

    //from (3): physically
    DirectoryHandler::removeDir(buildDirectory);
}

void Management::removeBuildLogically(int buildID){
    lock.lock();
    int index = -1;
    for(int i = 0; i < buildCount; i++){
        if(buildID == allBuilds[i].getBuildID()){
            index = i;
            break;
        }
    }

    if(index = -1){
        lock.unlock();
        return;//means the build has not been found
    }

    //one less build thus -1
    Build *newAllBuilds = new Build[buildCount-1];


    //copy over from 0 to index(where build was)
    int counter = 0;
    for(int i =0; i < index; i++){
        //create the build to be added
        Build oneNewBuild = Build(allBuilds[i].getBuildID(), allBuilds[i].getBuildName(), allBuilds[i].getBuildDescription(), allBuilds[i].getBuildDirectory());
        //add the build to new all builds
        newAllBuilds[counter] = oneNewBuild;
        counter++;
    }

    for(int i = index+1; i < buildCount; i++){
        //create the build to be added
        Build oneNewBuild = Build(allBuilds[i].getBuildID(), allBuilds[i].getBuildName(), allBuilds[i].getBuildDescription(), allBuilds[i].getBuildDirectory());
        //add the build to new all builds
        newAllBuilds[counter] = oneNewBuild;
        counter++;
    }

    //set the variables
    delete [] allBuilds;
    allBuilds = newAllBuilds;
    buildCount--;

    lock.unlock();
}
