#include "protosendstructure.h"
#include "Build.h"
#include "management.h"

ProtoSendStructure::ProtoSendStructure(QObject *parent)
    :Protocol(parent)
{
}

ProtoSendStructure::~ProtoSendStructure(){

}


void ProtoSendStructure::handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    if(!jsonObject.value("subHandler").toString().compare("BuildStructureRequest"))
        BuildStructureRequest(jsonObject, management, slaveSocket);
}


void ProtoSendStructure::BuildStructureRequest(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket){
    //get the structure for the build and send it across the network
    QString StringBuildID = jsonObject.value("buildID").toString();

    bool ok = false;

    int buildID = StringBuildID.toInt(&ok);

    if(!ok)
        return;

    QString buildDIR = management->getBuildByID(buildID)->getBuildDirectory();


    //go and get the structure
    QStringList structure = DirectoryHandler::getDirectoryStructure(buildDIR);

    //place the structure of directories inside a json format

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "ProtoSendStructure", true);
    appendJSONValue(jsonMessage, "subHandler","DuplicateStructure", true);
    appendJSONValue(jsonMessage, "buildID", StringBuildID, true);

    //start a new json value and append the list to it
    QString jsonStructureValue = "\"structure\" : {";
    for(int i = 0; i < structure.size(); i++){
        //append all the values as json
        QString directoryNumber = QString::number(i);
        QString aPath;

        //there is no comma at end of json values
        if(i == structure.size()-1)
            aPath = "\"" + directoryNumber + "\" : \"" + structure.at(i) + "\"";
        else
            aPath =  "\"" + directoryNumber + "\" : \"" + structure.at(i) + "\",";

        jsonStructureValue.append(aPath);
    }
    //end the list of structures(directory paths)
    jsonStructureValue.append("}");

    //append the list of all the structures
    jsonMessage.append(jsonStructureValue);

    endJSONMessage(jsonMessage);

    sendJSONMessage(slaveSocket, jsonMessage);
}
