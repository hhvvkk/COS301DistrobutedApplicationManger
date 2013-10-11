#include "appxmlwriter.h"

appXMLWriter::appXMLWriter()
{
    appXMLReader xRead;
    xRead.parseXML();
    appNames = xRead.getAppNames();
    appFiles = xRead.getAppFiles();
}

appXMLWriter::~appXMLWriter(){
    delete theXMLWriter;
}

void appXMLWriter::createXMLFile(){
    if(appNames.size() > 0){
        QFile file("apps.xml");

        if (!file.open(QIODevice::WriteOnly))
        {
        }
        else
        {
            theXMLWriter = new QXmlStreamWriter();
            theXMLWriter->setDevice(&file);
            theXMLWriter->writeStartDocument();
            theXMLWriter->writeStartElement("apps");

            QMapIterator<QString, QString> i(appNames);
            QMapIterator<QString, QString> j(appFiles);

            while (i.hasNext() && j.hasNext())
            {
                i.next(); j.next();
                theXMLWriter->writeStartElement("app");
                theXMLWriter->writeAttribute(i.key(), i.value());
                theXMLWriter->writeAttribute(j.key(), j.value());
                theXMLWriter->writeEndElement();
            }
            theXMLWriter->writeEndElement();
            theXMLWriter->writeEndDocument();
        }
    }
}

void appXMLWriter::receiveApp(QString AppName, QString appFile){
    appNames.insertMulti("appName",AppName);
    appFiles.insertMulti("appFile",appFile);
}
