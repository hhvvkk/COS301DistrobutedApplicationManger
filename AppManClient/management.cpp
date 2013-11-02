#include "management.h"

Management::Management()
{
    network = new Network(this);

    //if the directory for all builds does not exist, create it
    ///@todo change directory of builds...
    allBuildsDirectory = "builds";
    if(!QDir(allBuildsDirectory).exists())
        QDir().mkdir(allBuildsDirectory);

    readAppList();
}


Management::~Management()
{
    delete network;
    //delete xRead;
    while(allBuilds.size() > 0){
        Build *deleteABuild = allBuilds.takeAt(0);
        delete deleteABuild;
    }
}

void Management::connectToServer(QString ipAddress, int port){
    network->setConnectionDetails(ipAddress, port);
    network->connecToServer();
}

void Management::disconnectFromServer(){
    network->disconnect();
}

void Management::addBuild(Build *buildToAdd){
    //create the directory for that build if it does not exist
    createBuildDirectory(buildToAdd);

    allBuilds.append(buildToAdd);

}

void Management::createBuildDirectory(Build *build){
    QString newBuildDirectory = QString::number(build->getBuildID());
    if(!QDir(allBuildsDirectory+"/"+newBuildDirectory).exists())
    {
        QDir().mkdir(allBuildsDirectory+"/"+newBuildDirectory);
    }
    build->setBuildDirectory(allBuildsDirectory+"/"+newBuildDirectory);
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
        Build *newBuild = new Build(i.value().toInt(), j.value(), k.value(), l.value());
        this->addBuild(newBuild);
    }
}

Build *Management::getBuildByID(int id){
    lock.lock();
    for(int i = 0; i < allBuilds.size(); i++){
        if(allBuilds[i]->getBuildID() == id){
            lock.unlock();
            return allBuilds[i];
        }
    }

    lock.unlock();
    return 0;
}

QString Management::getBuildMD5(Build *build){
    BuildMD5 md5(build->getBuildDirectory(),5);
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
    Build *theBuild = getBuildByID(buildID);

    if(theBuild == 0){
        return;
    }

    theBuild->setBuildName(newBuildName);
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

    Build *theBuild = getBuildByID(buildID);

    if(theBuild == 0){
        return;
    }

    QString buildDirectory = theBuild->getBuildDirectory();

    //go and delete build
    //from (1): logical view point
    removeBuildLogically(buildID);

    //from (2): xml
    xmlWriter aWriter;
    aWriter.removeBuild(buildID);

    //from (3): physically
    DirectoryHandler::removeDir(buildDirectory);
}

Build * Management::getBuildAt(int i){
    if(i < 0 || i >= allBuilds.size())
        return 0;
    return allBuilds.at(i);
}

void Management::removeBuildLogically(int buildID){
    Build *theBuild = getBuildByID(buildID);

    if(theBuild == 0)
        return;

    allBuilds.removeOne(theBuild);

    delete theBuild;
}

void Management::readAppList(){
    appXMLReader xRead;
    xRead.parseXML();
    QMap<QString,QString> appName = xRead.getAppNames();
    QMap<QString,QString> appFile = xRead.getAppFiles();
    QMapIterator<QString, QString> m(appName);
    QMapIterator<QString, QString> n(appFile);

    while (m.hasNext() && n.hasNext())
    {
        m.next(); n.next();
        addToAppList(m.value(),n.value());
    }
}

void Management::runThisSim(QString build, QString recArg){
    QString runner = "";
        if(build.contains("App")){
            QString appName = build;

            appName.remove(0,4);
            QMapIterator<QString, QString> m(appList);
            while(m.hasNext()){
                m.next();
                if(m.key().compare(appName)==0){
                    runner = m.value();
                }
            }

        }
        QString app;
        QString arg;
        if(runner.compare("") == 0){
            build.chop(build.length() - build.indexOf("-"));
            Build *b = getBuildByID(build.toInt());
            app = /*"\"\"C:/AppManClient\"/"+*/b->getBuildDirectory();
            arg = "/"+recArg;
            QProcess* p = new QProcess();
            p->start(app+arg);
            p->waitForFinished(-1);
            p->kill();
            p->deleteLater();

        }else{
            app = "\"\""+runner+"\"\"";
            arg = "\" \""+recArg+"\"\"";

            QString data = app + arg;
            char * gg = data.toUtf8().data();

            system(gg);
        }


}


void Management::duplicateDirectoryStructure(QStringList masterDirectoryStructure, int intBuildID){

    QString allBuildsDirectory = "builds/";
    QString buildDir = allBuildsDirectory + "/" + QString::number(intBuildID)+ "/";
    QStringList thisDirectoryStructure = DirectoryHandler::getDirectoryStructure(buildDir);

    //compare masterDirectoryList with this one
    for(int i = 0; i < masterDirectoryStructure.size(); i++){
        //firstly create the directory path
        QDir().mkpath(buildDir + masterDirectoryStructure.at(i));

        //test if it is in slave but master already contains that subpath
        int totalSize = thisDirectoryStructure.size();
        for(int ti = 0; ti < totalSize; ti++){
            QString currentTIFolder = thisDirectoryStructure.at(ti);
            //if the master contains this path with subpaths
            if(isSubPath(masterDirectoryStructure.at(i), currentTIFolder)){
                thisDirectoryStructure.removeAt(ti);
                totalSize = thisDirectoryStructure.size();
            }
        }
    }

    //loop through the remainder of items(those that is not subpaths of the master builds
    int totalSize = thisDirectoryStructure.size();
    for(int i = 0; i < totalSize; i++){
        for(int ma = 0; ma < masterDirectoryStructure.size(); ma++){
            QString currentMasterFolder = masterDirectoryStructure.at(ma);
            //if the master has a dir with a subpath of the clients path
            //the path is subpath and size differs
            //means that it can be a wrong path
            if(isSubPath(currentMasterFolder, thisDirectoryStructure.at(i)) && (currentMasterFolder.size() != thisDirectoryStructure.size())){
                removeFolderStructure(buildDir, currentMasterFolder, thisDirectoryStructure.at(i));
                thisDirectoryStructure.removeAt(i);
                totalSize = thisDirectoryStructure.size();
                break;//there will be no more
            }
        }
    }
}

bool Management::isSubPath(QString onePath, QString subPath){

    if(onePath.size() < subPath.size())
        return false;

    for(int i = 0; i < subPath.size(); i++){
        if(onePath.at(i) != subPath.at(i)){
            return false;
        }
    }

    return true;
}

void Management::removeFolderStructure(QString buildPath, QString masterValue, QString slaveValue){
    qDebug()<<""<<masterValue<<" === === "<<slaveValue;
    QString theSeparator = "/";
    if(!masterValue.contains("/")){
        theSeparator = "\\";
    }
    while(slaveValue.size() > masterValue.size()){
        DirectoryHandler::removeDir(allBuildsDirectory + slaveValue);
        int removedDirLength = slaveValue.size() - slaveValue.lastIndexOf(theSeparator);
        slaveValue.remove(slaveValue.lastIndexOf(theSeparator), removedDirLength);
    }
}
