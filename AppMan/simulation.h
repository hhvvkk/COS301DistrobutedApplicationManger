#ifndef SIMULATION_H
#define SIMULATION_H

#include <QFile>
#include <QStringList>
#include <QString>

#include "Build.h"

class Simulation
{
public:
    Simulation();
    Simulation(QString name);
    ~Simulation();
    void addSlave(QStringList * slave, QStringList * build, QStringList * arg);
    void addName(QString name);
    QString getName() { return simName; }
    QStringList * getSlaves() { return slaves; }
    QStringList * getBuilds() { return builds; }
    QStringList * getArgs() { return args; }
private:
    QString simName;
    QStringList * slaves;
    QStringList * builds;
    QStringList * args;
};

#endif // SIMULATION_H
