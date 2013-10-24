#include "management.h"

const int MAX_MACHINES = 10000;

Management::Management(QObject *parent) :
    QObject(parent)
{
    server = 0;
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
    while(allBuilds.size() > 0){
        Build * build = allBuilds.takeAt(0);
        delete build;
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
        Database::instance()->insert_machine(uniqueID,address);
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

void Management::addBuild(Build *buildToAdd){
    allBuilds.append(buildToAdd);
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

Build *Management::getBuildByID(int id){
    buildLock.lock();
    for(int i = 0; i < allBuilds.size(); i++){
        if(allBuilds[i]->getBuildID() == id){
            buildLock.unlock();
            return allBuilds[i];
        }
    }

    buildLock.unlock();
    return 0;
}


bool Management::buildExistWithName(QString name){
    for(int i = 0; i < allBuilds.size(); i++){
        if(!name.compare(allBuilds[i]->getBuildName()))
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
    Build *build = getBuildByName(buildName);

    if(build == 0)
        return;

    lock->lock();

    Machine *machine = 0;
    for(int i = 0; i < machineCount; i++){
        if(getMachineAt(i)->getMachineID() == machineId){
            machine = getMachineAt(i);
            break;
        }
    }
    lock->unlock();

    bool hasBuild = machineWithBuild(machine->getMachineID(), build->getBuildID());

    //only add if the machine does not have the build
    if(!hasBuild)
        machine->copyBuildOver(*build);

}

bool Management::machineWithBuild(int machineID, int buildID){
    Build *build = getBuildByID(buildID);

    if(build == 0){
        return false;//there exist no such build
    }

    Machine *machine = getMachineById(machineID);

    if(machine == 0)
        return false;//there exist no such machine


    Build *machineBuilds = machine->getBuilds();

    for(int i = 0; i < machine->getBuildCount(); i++){
        if(machineBuilds[i].getBuildID() == buildID){
            return true;
        }
    }
    return false;
}


Build *Management::getBuildByName(QString name){
    for(int i = 0; i < allBuilds.size(); i++)
        if(!allBuilds[i]->getBuildName().compare(name))
            return allBuilds[i];

    return 0;
}

void Management::addBuildToSlave(int machineId, int BuildID, QString buildName){

    Machine *machine = getMachineById(machineId);

    if(machine == 0)
        return;

    Build *trueBuild = getBuildByID(BuildID);

    if(trueBuild == 0){
        //this point the build does not exist
        emit slaveGotBuild(machine, BuildID, buildName, false);
        //this point that machine has a build it is not suppose to have...go and delete it
        machine->deleteBuild(BuildID);

        //notify the slave that it should delete the build
        machine->deleteBuildNotify(BuildID);
    }
    else{
        Build buildToAdd = Build(BuildID, buildName, "", trueBuild->getBuildDirectory());

        bool existMachineWithBuild = machineWithBuild(machine->getMachineID(), buildToAdd.getBuildID());

        //if there already exist that build for the machine, don't add it twice
        if(existMachineWithBuild == false)
            machine->addBuild(buildToAdd);

        emit slaveGotBuild(machine, BuildID, buildName, true);

        //check if the build name corrolate with the information on the
        //slave side
        if(buildName.compare(trueBuild->getBuildName())){
            machine->updateBuildName(buildToAdd.getBuildID(), trueBuild->getBuildName());
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
    Build *theBuild = getBuildByID(BuildID);

    if(theBuild ==  0){
        //this point the build does not exist
        return;
    }

    QFuture<QString> future = QtConcurrent::run(this, &Management::getBuildMD5, theBuild);
    QString currentBuildMD5Value = future.result();


    if(!buildMD5Value.compare(currentBuildMD5Value)){
        setSlaveBuildIsSame(true, slaveId, theBuild->getBuildID());
    }
    else{
        setSlaveBuildIsSame(false, slaveId, theBuild->getBuildID());
    }
}


void Management::slaveABuildSize(int BuildID, QString buildMD5Value, int slaveId){
    Build *theBuild = getBuildByID(BuildID);

    if(theBuild == 0){
        //this point the build does not exist
        return;
    }


    Machine * m = getMachineById(slaveId);
    if(m == 0)
        return;
    QFuture<QString> future = QtConcurrent::run(this, &Management::getBuildMD5, theBuild);
    QString currentBuildMD5Value = future.result();

    if(!buildMD5Value.compare(currentBuildMD5Value)){
        setSlaveBuildIsSame(true, slaveId, theBuild->getBuildID());
    }
    else{
        setSlaveBuildIsSame(false, slaveId, theBuild->getBuildID());
    }

    m->slaveABuildSizeDone();
}


void Management::setSlaveBuildIsSame(bool isSame, int machineId, int buildID){
    Machine * slave = getMachineById(machineId);

    if(slave == 0)
        return;

    emit slaveBuildSizeSame(buildID, machineId, isSame);
    slave->setBuildSame(isSame, buildID);
}

// void slaveBuildSynched(int machineId, int buildId, double percentageSynched);
void Management::machineBuildSynched(int machineId, int buildId, double percentageSynched){
    if(percentageSynched < 0)
        percentageSynched = 0;
    if(percentageSynched > 100)
        percentageSynched = 100;
    emit slaveBuildSynched(machineId, buildId, percentageSynched);
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
    Build *theBuild = getBuildByID(buildID);

    if(theBuild == 0){
        return;
    }

    //else the build exists
    theBuild->setBuildDirectory(value);
    xmlWriter writer;
    writer.updateBuildDir(buildID, value);
}

void Management::setBuildName(int buildID, QString value){
    Build *theBuild = getBuildByID(buildID);

    if(theBuild == 0){
        return;
    }

    theBuild->setBuildName(value);
    xmlWriter writer;
    writer.updateBuildName(buildID, value);

    //notify all slaves apon the change
    for(int i = 0; i < machineCount; i++){
        Machine *machine = getMachineAt(i);

        if(machine == 0)
            continue;

        if(machineWithBuild(machine->getMachineID(), buildID)){
            //only notify the machine if that machine has the build
            machine->updateBuildName(buildID, value);
        }
    }
}

void Management::setBuildNumber(int buildID, QString value){
    Build *theBuild = getBuildByID(buildID);

    if(theBuild == 0){
        return;
    }

//    theBuild->setBuildDescription(value);
//    xmlWriter writer;
//    writer.updateBuildNumber(buildID, value);
}

void Management::setBuildDescription(int buildID, QString value){
    Build *theBuild = getBuildByID(buildID);

    if(theBuild == 0){
        return;
    }

    theBuild->setBuildDescription(value);
    xmlWriter writer;
    writer.updateBuildDescr(buildID, value);
}

void Management::addSimulation(Simulation *sim){
    simXMLWriter xWrite;
    xWrite.addSim(sim->getName(),sim->getSlaves(),sim->getBuilds(),sim->getArgs());
    qDebug()<<"about to add";
    xWrite.createXML();
    qDebug()<<"added";
    allSimulations.push_back(sim);
    simCount++;
}

void Management::deleteBuild(int buildID){

    //go and delete build
    //from (1):logical view point
    removeBuildLogically(buildID);

    //from (2):xml
    xmlWriter aWriter;
    aWriter.removeBuild(buildID);

    //from(3): all connected clients
    for(int i = 0; i < machineCount; i++){
        Machine *machine = getMachineAt(i);

        if(machine == 0)
            continue;

        //only notify the machine if that machine has the build
        machine->deleteBuildNotify(buildID);
    }

    //from(4):gui
    emit buildDeleted();
}

void Management::removeBuildLogically(int buildID){

    buildLock.lock();
    int index = -1;
    for(int i = 0; i < allBuilds.size(); i++){
        if(buildID == allBuilds[i]->getBuildID()){
            index = i;
            break;
        }
    }

    if(index == -1){
        buildLock.unlock();
        return;//means the build has not been found
    }
    Build *theBuildToDelete = allBuilds.at(index);
    allBuilds.removeOne(theBuildToDelete);

    //delete the build
    delete theBuildToDelete;

    buildLock.unlock();
}


void Management::slaveSuccessDeleteABuild(int machineID, int buildID){
    Machine *machine = getMachineById(machineID);

    machine->deleteBuild(buildID);

    emit slaveDeletedBuild(machineID, buildID);
}

void Management::deleteBuildFromSlave(int machineID, int buildID){
    Machine *machine = getMachineById(machineID);

    if(machine == 0)
        return;

    if(machineWithBuild(machineID, buildID)){
        machine->deleteBuildNotify(buildID);
    }
}

void Management::resynchAllBuildsOnSlave(int machineID){
    Machine *machine = getMachineById(machineID);

    if(machine == 0)
        return;

    machine->resynchAll();
}

void Management::resynchAllCertainBuild(int buildIDToResynch){
    Build *theBuild = getBuildByID(buildIDToResynch);

    if(theBuild == 0)
        return;

    for(int i = 0; i < machineCount; i++){
        Machine * machine = getMachineAt(i);
        if(machine == 0)
            continue;//continue to next in loop

        if(machineWithBuild(machine->getMachineID(), buildIDToResynch))
            machine->resynchAll();//invoke the resynch all in order to resynch all the builds currently there
    }
}
