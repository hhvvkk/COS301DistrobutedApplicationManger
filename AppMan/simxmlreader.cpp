#include "simxmlreader.h"

simXMLReader::simXMLReader()
{
    simCount = -1;
    readXML();
}

void simXMLReader::readXML(){
    QFile* file = new QFile("simulations.xml");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {}
    QXmlStreamReader xml(file);
    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        if(token == QXmlStreamReader::StartElement) {
            if(xml.name() == "simulations") {
                continue;
            }
            else if(xml.name() == "simulation") {
                simCount++;
                this->parseSimulation(xml);
                //qDebug()<<"1st parse";
                //qDebug()<<QString::number(simCount);
            }
            else{
                continue;
            }
        }
    }
    if(xml.hasError()) {
    }
    xml.clear();
}

void simXMLReader::parseSimulation(QXmlStreamReader &xml){
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "simulation") {
        return;
    }
    xml.readNextStartElement();
    QString name = xml.readElementText();
    QStringList * slaves = new QStringList();
    QStringList * builds = new QStringList();
    QStringList * args = new QStringList();
    //qDebug()<<name;
    simName.append(name);
    bool moreSlaves = true;
    while (moreSlaves)
    {
        xml.readNextStartElement();
        if(xml.name() == "slaves"){
            xml.readNextStartElement();
        }
        if(xml.name() == "slave"){
            xml.readNextStartElement();
            if(xml.name() == "slave"){
                xml.readNextStartElement();
            }
            if(xml.name() == "slaveID"){
                QString slave = xml.readElementText();
                slaves->append(slave);
                //qDebug()<<slave;
            }
            xml.readNextStartElement();
            if(xml.name() == "buildID"){
                QString build = xml.readElementText();
                builds->append(build);
                //qDebug()<<build;
            }
            xml.readNextStartElement();
            if(xml.name() == "args"){
                QString arg = xml.readElementText();
                args->append(arg);
                //qDebug()<<arg;
            }
            xml.readNextStartElement();
            if(xml.name() == "simulation"){
                reqSlaves.insert(simCount,slaves);
                reqBuilds.insert(simCount,builds);
                reqArgs.insert(simCount,args);
                //qDebug()<<"Hello:\t\t\t"<<QString::number(simCount);
                parseSimulation(xml);
            }
        }
        else{
            if(xml.name() == "simulation"){
                //if(reqSlaves.size() != 0){
                    reqSlaves.insert(simCount,slaves);
                    reqBuilds.insert(simCount,builds);
                    reqArgs.insert(simCount,args);
                //}
                //qDebug()<<"Hello:\t\t\t"<<QString::number(simCount);
                parseSimulation(xml);
            }
            moreSlaves = false;
        }

    }
}
