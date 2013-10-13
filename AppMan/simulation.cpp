#include "simulation.h"

Simulation::Simulation()
{
    simName = "DummyName";
    slaves = new QStringList();
    builds = new QStringList();
    args = new QStringList();
}

Simulation::Simulation(QString name){
    simName = name;
    slaves = new QStringList();
    builds = new QStringList();
    args = new QStringList();
}

Simulation::~Simulation(){
    simName = "";
}

void Simulation::addSlave(QStringList * slave, QStringList * build, QStringList * arg){
    slaves = slave;
    builds = build;
    args = arg;

}

void Simulation::addName(QString name){
    simName = name;
}

