#ifndef APPXMLWRITER_H
#define APPXMLWRITER_H

#include <QFile>
#include <QMap>
#include <QXmlStreamWriter>

#include "appxmlreader.h"

class appXMLWriter
{
public:
    appXMLWriter();
    ~appXMLWriter();
    void createXMLFile();
    void receiveApp(QString AppName, QString appFile);
private:
    QMap<QString,QString> appNames;
    QMap<QString,QString> appFiles;
    QXmlStreamWriter* theXMLWriter;
};

#endif // APPXMLWRITER_H
