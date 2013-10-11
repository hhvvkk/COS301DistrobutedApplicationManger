#ifndef APPXMLREADER_H
#define APPXMLREADER_H

#include <QDebug>
#include <QFile>
#include <QMap>
#include <QXmlStreamReader>

class appXMLReader
{
public:
    appXMLReader();
    void parseXML();
    QMap<QString,QString> getAppNames();
    QMap<QString,QString> getAppFiles();
private:
    void parseApp(QXmlStreamReader& xml);
    QMap<QString,QString> appNames;
    QMap<QString,QString> appFiles;
};

#endif // APPXMLREADER_H
