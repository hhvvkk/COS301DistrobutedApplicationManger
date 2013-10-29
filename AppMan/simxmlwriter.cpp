#include "simxmlwriter.h"

simXMLWriter::simXMLWriter()
{
    //simCount = 0;
    simXMLReader xRead;
    this->reqArgs = xRead.getReqArgs();
    this->reqBuilds = xRead.getReqBuilds();
    this->reqSlaves = xRead.getReqSlaves();
    this->simCount = xRead.getSimCount();
    this->simName = xRead.getSimNames();

}

void simXMLWriter::addSim(QString name, QStringList *slaves, QStringList *builds, QStringList *args){
    simName.append(name);
    reqSlaves.insertMulti(simCount,slaves);
    reqBuilds.insertMulti(simCount,builds);
    reqArgs.insertMulti(simCount,args);
    simCount++;
}

void simXMLWriter::createXML(){

    QFile file("simulations.xml");
    if (!file.open(QIODevice::WriteOnly)){}
    else{
        theXMLWriter = new QXmlStreamWriter();
        theXMLWriter->setDevice(&file);
        theXMLWriter->writeStartDocument();
        theXMLWriter->writeStartElement("simulations");
        for(int current = 0; current < simCount; current++){
            theXMLWriter->writeStartElement("simulation");
            theXMLWriter->writeTextElement("Simulation_Name",simName.at(current));
            theXMLWriter->writeStartElement("slaves");
            for(int i = 0; i < reqSlaves.value(current)->size(); i++){
                theXMLWriter->writeStartElement("slave");
                theXMLWriter->writeTextElement("slaveID",reqSlaves.value(current)->at(i));
                theXMLWriter->writeTextElement("buildID",reqBuilds.value(current)->at(i));
                theXMLWriter->writeTextElement("args",reqArgs.value(current)->at(i));
                theXMLWriter->writeEndElement();
            }
            theXMLWriter->writeEndElement();
            theXMLWriter->writeEndElement();
        }
        theXMLWriter->writeEndElement();
        theXMLWriter->writeEndDocument();
    }
}
