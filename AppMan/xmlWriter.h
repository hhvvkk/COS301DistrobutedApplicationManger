#ifndef XMLWRITER_H
#define XMLWRITER_H

#include <QXmlStreamWriter>
#include <QString>
#include <QMap>

class xmlWriter
{


public:
    xmlWriter();
    ~xmlWriter();
    //call this function to create xml
    void CreateXMLFile();
    QXmlStreamWriter* theXMLWriter;
    void receiveBuild(QString num,QString name,QString descript, QString direc);

private:
    QMap<QString,QString> buildNumber;
    QMap<QString,QString> buildName;
    QMap<QString,QString> buildDescription;
    QMap<QString,QString> buildDirectory;

};


#endif // XMLWRITER_H
