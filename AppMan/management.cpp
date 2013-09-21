#include "management.h"

Management::Management(QObject *parent) :
    QObject(parent)
{
    server = 0;
    buildCount = 0;
    machineCount = 0;
    lock = new QMutex();
}

void Management::setServer(Server *s){
    this->server = s;
}

Management::~Management(){
    delete server;
    if(buildCount != 0)
    {
        delete[] allBuilds;
    }
    if(machineCount != 0){
        for( QVector<Machine*>::iterator i = allMachines.begin(); i != allMachines.end(); ++i )
        {
            delete *i;
        }
            allMachines.clear();
    }
    delete lock;
}

void Management::addMachine(int uniqueID, QString address, ProtocolHandler *handler){
    lock->lock();//lock the critical sections

    if(uniqueID <= 0){
        //thus generate a unique id
        uniqueID = generateUniqueId();
    }

    Machine *machine = new Slave(uniqueID, address);
    machine->setProtocolHandler(handler);
    handler->setMachine(machine);

    allMachines.push_back(machine);
    machineCount++;
    //emit the signal so it can be used by main form
    emit newSlaveConnected(machine);

    lock->unlock();//finally unlock for the critical section
}

void Management::removeMachine(Machine *m){
    lock->lock();

    if(m == 0)
        return;

    int index = -1;
    if(allMachines.size() == 0){
        return;
    }
    for(unsigned int i = 0; i < allMachines.size(); i++)
        if(m == allMachines.at(i)){
            index = i;
        }

    if(index == -1)//means its not found
        return;

    allMachines.erase(allMachines.begin()+index,allMachines.begin()+(index+1));
    machineCount--;

    //delete the machine
    emit slaveDisconnected(m->getMachineID());
    m->~Machine();
    lock->unlock();
}

void Management::addBuild(Build buildToAdd){
    if(buildCount != 0){
        Build temp[buildCount];
        for(int i = 0; i < buildCount; i++){
            temp[i] = allBuilds[i];
        }
        allBuilds = new Build[buildCount+1];
        for(int j = 0; j < buildCount; j++){
            allBuilds[j] = temp[j];
        }
        allBuilds[buildCount] = buildToAdd;
    }
    else{
        allBuilds = new Build[buildCount+1];
        allBuilds[buildCount] = buildToAdd;
    }
    buildCount++;
    //qDebug("added build to management");
}

Machine* Management::getMachineAt(int i){
    if(i<0 || i >= (int) allMachines.size())
        return 0;
    return allMachines.at(i);
}

void Management::startServer(){
    if(server == NULL){
        qDebug()<<"Server is NULL";
        return;
    }
    server->startServer();
}

void Management::stopServer(){
    server->stopServer();
}

void Management::setPort(int newPort){
    server->setPort(newPort);
}

Build Management::getBuildByID(int id){
    for(int i = 0; i < buildCount; i++){
        if(allBuilds[i].getBuildID() == id){
            return allBuilds[i];
        }
    }
    // Returns a build with id 0 to show it does not exist
    Build b(0,"NULL","NULL","NULL");
    return b;
}


bool Management::buildExistWithName(QString name){
    for(int i = 0; i < buildCount; i++){
        if(!name.compare(allBuilds[i].getBuildName()))
            return true;
    }
    return false;
}

bool Management::machineExistWithIp(QString ip){
    lock->lock();
    for(int i = 0; i < machineCount; i++){
        if(!ip.compare(allMachines[i]->getMachineIP())){
            lock->unlock();
            return true;
        }
    }
    lock->unlock();
    return false;
}

void Management::copyBuildOver(int machineId, QString buildName){
    Build build = getBuildByName(buildName);

    lock->lock();

    Machine *machine = 0;
    for(int i = 0; i < machineCount; i++){
        if(getMachineAt(i)->getMachineID() == machineId){
            machine = getMachineAt(i);
            break;
        }
    }

    lock->unlock();

    machine->copyBuildOver(build);

}


Build Management::getBuildByName(QString name){
    for(int i = 0; i < buildCount; i++)
        if(!allBuilds[i].getBuildName().compare(name))
            return allBuilds[i];

    return Build(0, "NULL", "NULL", "NULL");
}

void Management::addBuildToSlave(int machineId, int buildNo, QString buildName){

    Machine *machine = getMachineById(machineId);

    if(machine==NULL)
        return;

    Build buildToAdd = getBuildByID(buildNo);

    if(!buildToAdd.getBuildDescription().compare("NULL")
        && !buildToAdd.getBuildDirectory().compare("NULL")
        && !buildToAdd.getBuildName().compare("NULL")
        && buildToAdd.getBuildID() == 0){
        //this point the build does not exist
        emit slaveGotBuild(machine, buildNo, buildName, false);
    }
    else{
        machine->addBuild(buildToAdd);
        emit slaveGotBuild(machine, buildNo, buildName, true);
    }

}

QString Management::getBuildMD5(Build* build){
/*  This method iterates through all files and sub-directories
 *  of the current build directory, adding all file data and then
 *  calculates a MD5 checksum of all files combined
 */
    QCryptographicHash md5(QCryptographicHash::Md5);

    QString dir = build->getBuildDirectory();

    myDirIterator dirIt(dir,1);
    dirIt.getFileInfo();
    QVector<QString> paths = dirIt.retrieveFilePaths();

    for (int i=0;i<paths.size();i++) {
        QFile file(paths.at(i));
        //qDebug()<< "PATH: " + paths.at(i);
        file.open(QFile::ReadOnly);
        md5.addData(file.readAll());
        file.close();
    }
    //qDebug()<< md5.result().toHex();
    QString hash(md5.result().toHex().constData());
    return hash;


/*  First Attempt - Trying to calculate hash on sinlge folder
 *  Gives same value even on update
 *
 *  QString dir = build->getBuildDirectory();
 *  QFile file(dir);
 *  file.open(QFile::ReadOnly);
 *  QByteArray ba = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5).toHex();
 *  qDebug()<< ba;
 *  file.close();
 *  QString hash(ba.constData());
 *  return hash;
 */
}


Machine *Management::getMachineById(int machineId){
    lock->lock();

    Machine *machine = 0;
    for(int i = 0; i < machineCount; i++){
        if(allMachines.at(i)->getMachineID() == machineId){
            machine = allMachines.at(i);
            break;
        }
    }

    lock->unlock();
    return machine;
}

void Management::slaveBuildSize(int buildNo, QString buildMD5Value, int slaveId){
    Build theBuild = getBuildByID(buildNo);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        //this point the build does not exist
        return;
    }
    else{
        //qDebug()<<"EXISTS::"<<buildNo;
    }

    if(!buildMD5Value.compare(getBuildMD5(&theBuild))){
        setSlaveBuildIsSame(true, slaveId, theBuild.getBuildID());
        //void slaveBuildSizeSame(int buildId, int slaveId, bool isTheSame);
        emit slaveBuildSizeSame(theBuild.getBuildID(), slaveId, true);
    }
    else{
        setSlaveBuildIsSame(false, slaveId, theBuild.getBuildID());
        emit slaveBuildSizeSame(theBuild.getBuildID(), slaveId, false);
    }
}


void Management::slaveABuildSize(int buildNo, QString buildMD5Value, int slaveId){
    Build theBuild = getBuildByID(buildNo);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        //this point the build does not exist
        return;
    }
    else{
        //qDebug()<<"EXISTS::"<<buildNo;
    }

    if(!buildMD5Value.compare(getBuildMD5(&theBuild))){
        setSlaveBuildIsSame(true, slaveId, theBuild.getBuildID());
        //void slaveBuildSizeSame(int buildId, int slaveId, bool isTheSame);
        emit slaveBuildSizeSame(theBuild.getBuildID(), slaveId, true);
    }
    else{
        setSlaveBuildIsSame(false, slaveId, theBuild.getBuildID());
        emit slaveBuildSizeSame(theBuild.getBuildID(), slaveId, false);
    }

    Machine * m = getMachineById(slaveId);
    m->slaveABuildSizeDone(theBuild.getBuildID());
}


void Management::setSlaveBuildIsSame(bool isSame, int machineId, int buildID){
    Machine * slave = getMachineById(machineId);

    slave->setBuildSame(isSame, buildID);
}

// void slaveBuildSynched(int machineId, int buildId, double percentageSynched);
void Management::machineBuildSynched(int machineId, int buildId, double percentageSynched){
    emit slaveBuildSynched(machineId, buildId, percentageSynched);
}

void Management::setDetStats(QString stats){
    detStats = stats;
}

void Management::setMinStats(QString stats){
    minStats = stats;
}

int Management::generateUniqueId(){
    return 1;
}
