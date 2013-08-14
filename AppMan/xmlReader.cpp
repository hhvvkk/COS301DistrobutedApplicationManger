#include "xmlReader.h"

xmlReader::xmlReader(){

}

xmlReader::~xmlReader(){

}

void xmlReader::parseXML(){
    QFile* file = new QFile("builds.xml");
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
            if(xml.name() == "builds") {
                continue;
            }
            if(xml.name() == "build") {
                this->parseBuild(xml);
            }
        }
    }
    if(xml.hasError()) {
        qDebug()<<"There is an error in the XML";
    }
    xml.clear();
}

void xmlReader::parseBuild(QXmlStreamReader& xml) {
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() == "build") {
        return;
    }
    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("buildNumber")) {
        buildNumber.insertMulti("buildNumber",attributes.value("buildNumber").toString());
    }
    if(attributes.hasAttribute("buildName")) {
        buildName.insertMulti("buildName",attributes.value("buildName").toString());
    }
    if(attributes.hasAttribute("buildDescription")) {
        buildDescription.insertMulti("buildDescription",attributes.value("buildDescription").toString());
    }
    if(attributes.hasAttribute("buildDirectory")) {
        buildDirectory.insertMulti("buildDirectory",attributes.value("buildDirectory").toString());
    }
    xml.readNext();
}

QMap<QString,QString> xmlReader::getBuildNumber(){
    return buildNumber;
}

QMap<QString,QString> xmlReader::getBuildName(){
    return buildName;
}

QMap<QString,QString> xmlReader::getBuildDescription(){
    return buildDescription;
}

QMap<QString,QString> xmlReader::getBuildDirectory(){
    return buildDirectory;
}
