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

void Management::addMachine(Machine* machine){
    lock->lock();//lock the critical sections

    allMachines.push_back(machine);
    machineCount++;
    //emit the signal so it can be used by main form
    emit newSlaveConnected(machine ,machineCount-1);

    lock->unlock();//finally unlock for the critical section
}

void Management::removeMachine(Machine *m){
    lock->lock();

    qDebug()<<"attempting removal of machine";
    int index = -1;
    if(allMachines.size() == 0){
        qDebug()<<"size = 0";
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

    emit slaveDisconnected(m,index);

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
    qDebug("added build to management");
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
