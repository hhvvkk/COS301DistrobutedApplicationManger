#include "appxmlreader.h"

appXMLReader::appXMLReader()
{
}

void appXMLReader::parseXML(){
    QFile* file = new QFile("apps.xml");
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"XML File not here";
    }
    QXmlStreamReader xml(file);
    while(!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        if(token == QXmlStreamReader::StartElement) {
            if(xml.name() == "apps") {
                continue;
            }
            if(xml.name() == "app") {
                this->parseApp(xml);
            }
        }
    }
    if(xml.hasError()) {
        qDebug()<<"There is an error in the XML";
    }
    xml.clear();
}

void appXMLReader::parseApp(QXmlStreamReader &xml){
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "app") {
        return;
    }
    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("appName")) {
        appNames.insertMulti("appName",attributes.value("appName").toString());
        qDebug()<<attributes.value("appName").toString();
    }
    if(attributes.hasAttribute("appFile")) {
        appFiles.insertMulti("appFile",attributes.value("appFile").toString());
        qDebug()<<attributes.value("appFile").toString();
    }

    xml.readNext();
}

QMap<QString,QString> appXMLReader::getAppNames(){
    return appNames;
}

QMap<QString,QString> appXMLReader::getAppFiles(){
    return appFiles;
}
