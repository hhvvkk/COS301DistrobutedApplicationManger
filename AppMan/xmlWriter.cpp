#include "xmlWriter.h"
#include <QMap>
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QMapIterator>
#include <QDebug>

xmlWriter::xmlWriter()
{


}

void xmlWriter::CreateXMLFile()
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
qDebug()<<"creating";
    }
}

xmlWriter::~xmlWriter()
{
    delete theXMLWriter;
}

void xmlWriter::receiveBuild(QString num,QString name,QString descript, QString direc){
    qDebug()<<"receiving";
    buildNumber.insertMulti("buildNumber",num);
    buildName.insertMulti("buildName",name);
    buildDescription.insertMulti("buildDescription",descript);
    buildDirectory.insertMulti("buildDirectory",direc);
    qDebug()<<"done receiving";
}
