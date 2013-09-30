#include "xmlWriter.h"

xmlWriter::xmlWriter()
{
    xmlReader xRead;
    xRead.parseXML();
    this->buildNumber = xRead.getBuildNumber();
    this->buildName = xRead.getBuildName();
    this->buildDescription = xRead.getBuildDescription();
    this->buildDirectory = xRead.getBuildDirectory();
}

void xmlWriter::createXMLFile()
{
    QFile file("builds.xml");

    if (!file.open(QIODevice::WriteOnly))
    {
  //  QMessageBox::warning(0, "Read only", "The file is in read only mode");
    }
    else
    {

        theXMLWriter = new QXmlStreamWriter();
        theXMLWriter->setDevice(&file);
        theXMLWriter->writeStartDocument();
        theXMLWriter->writeStartElement("builds");

        QMapIterator<QString, QString> i(buildNumber);
        QMapIterator<QString, QString> j(buildName);
        QMapIterator<QString, QString> k(buildDescription);
        QMapIterator<QString, QString> l(buildDirectory);

        while (i.hasNext() && j.hasNext() && k.hasNext() && l.hasNext())
        {
            i.next(); j.next(); k.next(); l.next();
            theXMLWriter->writeStartElement("build");
            theXMLWriter->writeAttribute(i.key(), i.value());
            theXMLWriter->writeAttribute(j.key(), j.value());
            theXMLWriter->writeAttribute(k.key(), k.value());
            theXMLWriter->writeAttribute(l.key(), l.value());
            theXMLWriter->writeEndElement();
        }
        theXMLWriter->writeEndElement();
        theXMLWriter->writeEndDocument();
        qDebug()<<"creating xml";
    }

}

xmlWriter::~xmlWriter()
{
    delete theXMLWriter;
}

void xmlWriter::receiveBuild(QString num,QString name,QString descript, QString direc){
    //qDebug()<<"receiving build";
    buildNumber.insertMulti("buildNumber",num);
    buildName.insertMulti("buildName",name);
    buildDescription.insertMulti("buildDescription",descript);
    buildDirectory.insertMulti("buildDirectory",direc);
    //qDebug()<<"done receiving build";
}


int xmlWriter::findBuildIndex(int buildID){

    QMapIterator<QString, QString> mapIterator = QMapIterator<QString, QString>(buildNumber);

    if(!mapIterator.hasNext()){
        return -1;
    }

    QMapIterator<QString, QString> mapI(buildNumber);

    int count = 0;
    while (mapI.hasNext())
    {
        mapI.next();
        if(!mapI.value().compare(QString::number(buildID)))
            return count;
        count++;
    }

    return -1;
}

void xmlWriter::updateBuildName(int buildID, QString newBuildName){
    int index = findBuildIndex(buildID);

    //qDebug()<<"IndexFound = "<<index;

    if(index <= -1 || index >= buildNumber.values().size()){
        return;
    }

    //buildName.values().at(index);
    buildName.values().replace(index, newBuildName);
    qDebug()<<"newBuildNAme"<<newBuildName;
    createXMLFile();
}
