#include "management.h"
#include <QDebug>

Management::Management(QObject *parent) :
    QObject(parent)
{
    server = 0;
    buildCount = 0;
    machineCount = 0;
}

void Management::setServer(Server *s){
    this->server = s;
}

Management::~Management()
{
    delete server;
    if(buildCount != 0)
    {
        delete[] allBuilds;
    }
    if(machineCount != 0){
        for( vector<Machine*>::iterator i = allMachines.begin(); i != allMachines.end(); ++i )
        {
            delete *i;
        }
            allMachines.clear();
    }

}


void Management::addMachine(Master masterToAdd){
    allMachines.push_back(new Master(masterToAdd));

    machineCount++;
}

void Management::addMachine(Slave *slaveToAdd){
    allMachines.push_back(slaveToAdd);
    machineCount++;
}

void Management::addMachine(Machine* machine){
    allMachines.push_back(machine);
    machineCount++;
    //emit the signal so it can be used by main form
    emit newSlaveConnected();
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
    qDebug("added");
}


Machine* Management::getMachineAt(int i){
    if(i<0 || i >= allMachines.size())
        return 0;
    return allMachines.at(i);
}

void Management::startServer(){
    if(server == NULL){
        qDebug()<<"FASD";
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

void Management::setSlaveOffline(Machine *m, bool isOffline){
    if(isOffline){
        m->setMachineOnline(false);
    }
    else{
        m->setMachineOnline(true);
    }
    emit slaveDisconnected();
}

void Management::removeMachine(Machine *m){
    qDebug()<<"attempting removal";
    int index = -1;
    if(allMachines.size() == 0){
        qDebug()<<"size = 0";
        return;
    }
    for(int i = 0; i < allMachines.size(); i++)
        if(m == allMachines.at(i)){
            index = i;
        }

    if(index == -1)//means its not found
        return;

    allMachines.erase(allMachines.begin()+index,allMachines.begin()+(index+1));
    machineCount--;
}

Build Management::getBuildByID(int id){
    for(int i = 0; i < buildCount; i++){
        if(allBuilds[i].getBuildID() == id){
            return allBuilds[i];
        }
        //Need an else statement...
    }
}
