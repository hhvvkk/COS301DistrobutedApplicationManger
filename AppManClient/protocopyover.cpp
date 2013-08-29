#include "protocopyover.h"
#include "Build.h"
#include "xmlWriter.h"
#include "management.h"

ProtoCopyOver::ProtoCopyOver(QObject *parent)
    :Protocol(parent)
{
}

void ProtoCopyOver::handle(QString data, Management *management, QTcpSocket *masterSocket) {
    if(data.contains("CopyBuildOver:#"))
        CopyBuildOver(data, management, masterSocket);
}

void ProtoCopyOver::CopyBuildOver(QString data, Management *management, QTcpSocket *masterSocket){
    //E.g CopyBuildOver:#1#NameBlah
    //15 = size of 'CopyBuildOver:#'

    QString rightSide = data.right((data.size()-15));
    //E.g. RIGHT SIDE= "1#NameBlah"


    //REASONING behind this method(THE INDEX COUNTING)
    //is that once you have for instance multiple # in name...
    //this prevents problems from occurring
    QString buildNo = rightSide.left(rightSide.indexOf("#"));
    // +1 to account for # as well
    QString buildName = rightSide.right(rightSide.length() - (buildNo.length()+1));
    Build newBuild = Build(buildNo.toInt() , buildName,"", "");

    xmlWriter xWrite;
    Build buildToAdd = management->createBuildDirectory(newBuild);
    xWrite.receiveBuild(QString::number(buildToAdd.getBuildID()),buildToAdd.getBuildName(),buildToAdd.getBuildDescription(),buildToAdd.getBuildDirectory());
    xWrite.CreateXMLFile();
    management->addBuild(buildToAdd);

    QString buildAddedMessage = "||GotABuild:#"+buildNo+"||";
    masterSocket->write(buildAddedMessage.toAscii().data());
    masterSocket->flush();//write all that should be written
}
