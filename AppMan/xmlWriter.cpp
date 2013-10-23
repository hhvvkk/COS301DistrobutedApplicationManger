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
    }

}

xmlWriter::~xmlWriter()
{
    delete theXMLWriter;
}

void xmlWriter::receiveBuild(QString num,QString name,QString descript, QString direc){
    buildNumber.insertMulti("buildNumber",num);
    buildName.insertMulti("buildName",name);
    buildDescription.insertMulti("buildDescription",descript);
    buildDirectory.insertMulti("buildDirectory",direc);
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

    if(index <= -1 || index >= buildNumber.values().size()){
        return;
    }

    QString buildDescr = buildDescription.values().at(index);
    QString buildDir = buildDirectory.values().at(index);
    QString buildNum = buildNumber.values().at(index);

    RemoveBuildToBeUpdated(buildID);

    buildNumber.insertMulti("buildNumber",buildNum);
    buildName.insertMulti("buildName",newBuildName);
    buildDescription.insertMulti("buildDescription",buildDescr);
    buildDirectory.insertMulti("buildDirectory",buildDir);

    createXMLFile();
}

void xmlWriter::updateBuildDir(int buildID, QString newBuildDir){
    int index = findBuildIndex(buildID);

    if(index <= -1 || index >= buildNumber.values().size()){
        return;
    }

    QString theBuildName = buildName.values().at(index);
    QString theBuildDescr = buildDescription.values().at(index);
    QString theBuildNum = buildNumber.values().at(index);

    RemoveBuildToBeUpdated(buildID);

    buildNumber.insertMulti("buildNumber",theBuildNum);
    buildName.insertMulti("buildName",theBuildName);
    buildDescription.insertMulti("buildDescription",theBuildDescr);
    buildDirectory.insertMulti("buildDirectory",newBuildDir);

    createXMLFile();
}

void xmlWriter::updateBuildDescr(int buildID, QString newBuildDescr){

    int index = findBuildIndex(buildID);

    if(index <= -1 || index >= buildNumber.values().size()){
        return;
    }

    QString build_Name = buildName.values().at(index);
    QString buildDir = buildDirectory.values().at(index);
    QString buildNum = buildNumber.values().at(index);

    RemoveBuildToBeUpdated(buildID);

    buildNumber.insertMulti("buildNumber",buildNum);
    buildName.insertMulti("buildName",build_Name);
    buildDescription.insertMulti("buildDescription",newBuildDescr);
    buildDirectory.insertMulti("buildDirectory",buildDir);

    createXMLFile();
}

void xmlWriter::RemoveBuildToBeUpdated(int buildID){

    QMap <QString,QString> tmpNum;
    QMap <QString,QString> tmpName;
    QMap <QString,QString> tmpDesc;
    QMap <QString,QString> tmpDir;

    QMapIterator<QString, QString> i(buildNumber);
    QMapIterator<QString, QString> j(buildName);
    QMapIterator<QString, QString> k(buildDescription);
    QMapIterator<QString, QString> l(buildDirectory);

    int m = 0;

    while (i.hasNext() && j.hasNext() && k.hasNext() && l.hasNext())
    {
        i.next(); j.next(); k.next(); l.next();

        if(buildNumber.values().at(m).compare(QString::number(buildID))){
            tmpNum.insertMulti("buildNumber",buildNumber.values().at(m));
            tmpName.insertMulti("buildName",buildName.values().at(m));
            tmpDesc.insertMulti("buildDescription",buildDescription.values().at(m));
            tmpDir.insertMulti("buildDirectory",buildDirectory.values().at(m));
        }
        m++;
    }
    buildNumber = tmpNum;
    buildName = tmpName;
    buildDescription = tmpDesc;
    buildDirectory = tmpDir;
}


void xmlWriter::removeBuild(int buildID){
    QMap <QString,QString> tmpNum;
    QMap <QString,QString> tmpName;
    QMap <QString,QString> tmpDesc;
    QMap <QString,QString> tmpDir;

    QMapIterator<QString, QString> i(buildNumber);
    QMapIterator<QString, QString> j(buildName);
    QMapIterator<QString, QString> k(buildDescription);
    QMapIterator<QString, QString> l(buildDirectory);

    int m = 0;

    while (i.hasNext() && j.hasNext() && k.hasNext() && l.hasNext())
    {
        i.next(); j.next(); k.next(); l.next();

        if(buildNumber.values().at(m).compare(QString::number(buildID))){
            //if the build is not the same include the build
            tmpNum.insertMulti("buildNumber",buildNumber.values().at(m));
            tmpName.insertMulti("buildName",buildName.values().at(m));
            tmpDesc.insertMulti("buildDescription",buildDescription.values().at(m));
            tmpDir.insertMulti("buildDirectory",buildDirectory.values().at(m));
        }
        m++;
    }

    //replace the current values
    buildNumber = tmpNum;
    buildName = tmpName;
    buildDescription = tmpDesc;
    buildDirectory = tmpDir;

    createXMLFile();
}
