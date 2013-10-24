#ifndef SIMXMLWRITER_H
#define SIMXMLWRITER_H

#include <QMap>
#include <QStringList>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>

#include "simxmlreader.h"

class simXMLWriter
{
public:
    simXMLWriter();
    void addSim(QString name, QStringList *slaves, QStringList *builds, QStringList *args);
    void createXML();


private:
    QStringList simName;
    QMap<int, QStringList*> reqSlaves;
    QMap<int, QStringList*> reqBuilds;
    QMap<int, QStringList*> reqArgs;
    int simCount;
    QXmlStreamWriter* theXMLWriter;
};

#endif // SIMXMLWRITER_H
