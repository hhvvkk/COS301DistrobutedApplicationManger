#include "management.h"

const int MAX_MACHINES = 10000;

Management::Management(QObject *parent) :
    QObject(parent)
{
    server = 0;
    buildCount = 0;
    machineCount = 0;
    lock = new QMutex();
    simCount = 0;
}

void Management::setServer(Server *s){
    this->server = s;
}

Management::~Management(){
    if(server != 0){
        server->stopServer();
        delete server;
    }
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

    bool updatedMachineiD = false;
    if(uniqueID <= 0){
        updatedMachineiD = true;
        //thus generate a unique id
        uniqueID = generateUniqueId();
    }

    if(handler == 0){
        lock->unlock();
        return;
    }

    Machine *machine = new Slave(uniqueID, address);
    machine->setProtocolHandler(handler);
    handler->setMachine(machine);

    if(updatedMachineiD){
        //go and update the machine id on the machine
        machine->updateUniqueID(uniqueID);
    }

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
    delete m;
    lock->unlock();
}

void Management::addBuild(Build buildToAdd){
    for(int i = 0; i < buildCount; i++){
        if(buildToAdd.getBuildID() == allBuilds[i].getBuildID())
            return;//return if it tries to add something which should not be able to
    }
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

Build &Management::getBuildByID(int id){
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

void Management::addBuildToSlave(int machineId, int BuildID, QString buildName){

    Machine *machine = getMachineById(machineId);

    if(machine==NULL)
        return;

    Build &trueBuild = getBuildByID(BuildID);

    if(!trueBuild.getBuildDescription().compare("NULL")
        && !trueBuild.getBuildDirectory().compare("NULL")
        && !trueBuild.getBuildName().compare("NULL")
        && trueBuild.getBuildID() == 0){
        //this point the build does not exist
        emit slaveGotBuild(machine, BuildID, buildName, false);
        //this point that machine has a build it is not suppose to have...go and delete it
    }
    else{
        Build buildToAdd = Build(BuildID, buildName, "", trueBuild.getBuildDirectory());
        machine->addBuild(buildToAdd);
        emit slaveGotBuild(machine, BuildID, buildName, true);

        //check if the build name corrolate with the information on the
        //slave side
        if(buildName.compare(trueBuild.getBuildName())){
            machine->updateBuildName(buildToAdd.getBuildID(), trueBuild.getBuildName());
        }
    }

}

QString Management::getBuildMD5(Build* build){
//    BuildMD5 *md5 = new BuildMD5(build->getBuildDirectory(),5);
//    QFuture <void>futureValue = QtConcurrent::run(md5, &BuildMD5::generate);

//    //wait until it has completed
//    futureValue.waitForFinished();
//    QString returnValue = md5->getDirectoryMD5();

//    md5->deleteLater();
//    return returnValue;
    BuildMD5 md5(build->getBuildDirectory(),5);
    md5.generate();
    return md5.getDirectoryMD5();
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

void Management::slaveBuildSize(int BuildID, QString buildMD5Value, int slaveId){
    Build &theBuild = getBuildByID(BuildID);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        //this point the build does not exist
        return;
    }

    QFuture<QString> future = QtConcurrent::run(this, &Management::getBuildMD5, &theBuild);
    QString currentBuildMD5Value = future.result();


    if(!buildMD5Value.compare(currentBuildMD5Value)){
        setSlaveBuildIsSame(true, slaveId, theBuild.getBuildID());
        //void slaveBuildSizeSame(int buildId, int slaveId, bool isTheSame);
        emit slaveBuildSizeSame(theBuild.getBuildID(), slaveId, true);
    }
    else{
        setSlaveBuildIsSame(false, slaveId, theBuild.getBuildID());
        emit slaveBuildSizeSame(theBuild.getBuildID(), slaveId, false);
    }
}


void Management::slaveABuildSize(int BuildID, QString buildMD5Value, int slaveId){
    Build &theBuild = getBuildByID(BuildID);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        //this point the build does not exist
        return;
    }


    Machine * m = getMachineById(slaveId);
    if(m == 0)
        return;
    QFuture<QString> future = QtConcurrent::run(this, &Management::getBuildMD5, &theBuild);
    QString currentBuildMD5Value = future.result();

    if(!buildMD5Value.compare(currentBuildMD5Value)){
        setSlaveBuildIsSame(true, slaveId, theBuild.getBuildID());
        //void slaveBuildSizeSame(int buildId, int slaveId, bool isTheSame);
        emit slaveBuildSizeSame(theBuild.getBuildID(), slaveId, true);
    }
    else{
        setSlaveBuildIsSame(false, slaveId, theBuild.getBuildID());
        emit slaveBuildSizeSame(theBuild.getBuildID(), slaveId, false);
    }

    m->slaveABuildSizeDone();
}


void Management::setSlaveBuildIsSame(bool isSame, int machineId, int buildID){
    Machine * slave = getMachineById(machineId);

    if(slave == 0)
        return;

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
    for (int i=1 ;i <= MAX_MACHINES ;i++) {
        if (Database::instance()->checkUniqueID(i)) {
            return i;
        }
    }
    return -1;
}


void Management::slaveUpdatedBuildName(int machineID, int buildID, QString buildName){
    emit newSlaveUpdatedBuildName(machineID, buildID, buildName);
}



void Management::setBuildDirectory(int buildID, QString value){
    Build &theBuild = getBuildByID(buildID);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        return;
    }

    //else the build exists
    theBuild.setBuildDirectory(value);
    xmlWriter writer;
    writer.updateBuildDir(buildID, value);
}

void Management::setBuildName(int buildID, QString value){
    Build &theBuild = getBuildByID(buildID);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        return;
    }

    theBuild.setBuildName(value);
    xmlWriter writer;
    writer.updateBuildName(buildID, value);
}

void Management::setBuildNumber(int buildID, QString value){
    Build &theBuild = getBuildByID(buildID);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        return;
    }

    theBuild.setBuildDescription(value);
    xmlWriter writer;
    writer.updateBuildDir(buildID, value);
}

void Management::setBuildDescription(int buildID, QString value){
    Build &theBuild = getBuildByID(buildID);

    if(!theBuild.getBuildDescription().compare("NULL")
        && !theBuild.getBuildDirectory().compare("NULL")
        && !theBuild.getBuildName().compare("NULL")
        && theBuild.getBuildID() == 0){
        return;
    }

    theBuild.setBuildDescription(value);
    xmlWriter writer;
    writer.updateBuildDescr(buildID, value);
}

void Management::addSimulation(Simulation *sim){
    allSimulations.push_back(sim);
    simCount++;
}

void Management::deleteBuild(int buildID){
    if(buildID < 0){
        return;
    }

    //go and delete build
    //from (1):allBuilds

    //from (2):xml

    //from (3):all connected clients

}
