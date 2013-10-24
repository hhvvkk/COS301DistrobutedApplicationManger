#ifndef SIMXMLREADER_H
#define SIMXMLREADER_H

#include <QXmlStreamReader>
#include <QStringList>
#include <QMap>
#include <QFile>
//#include <QDebug>

class simXMLReader
{
public:
    simXMLReader();
    QStringList getSimNames() {return simName;}
    QMap<int, QStringList*> getReqSlaves() {return reqSlaves;}
    QMap<int, QStringList*> getReqBuilds() {return reqBuilds;}
    QMap<int, QStringList*> getReqArgs() {return reqArgs;}
    int getSimCount() {return simCount+1;}

private:
    void readXML();
    void parseSimulation(QXmlStreamReader& xml);

    QStringList simName;
    QMap<int, QStringList*> reqSlaves;
    QMap<int, QStringList*> reqBuilds;
    QMap<int, QStringList*> reqArgs;
    int simCount;
};

#endif // SIMXMLREADER_H
