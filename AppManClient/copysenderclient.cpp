#include "copysenderclient.h"
#include <json.h>
#include <QVariant>
#include <QVariantMap>


CopySenderClient::CopySenderClient(QHostAddress hAdr, int portNumber, QObject *parent) :
    QObject(parent),
    port(portNumber),
    hostAddress(hAdr)
{
    socket = new QTcpSocket();
    firstTalk = true;
    //connect(socket, SIGNAL(connected()), socketClient, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadFunction()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedFunction()));
    allBuildsDirectory = "builds";
}

CopySenderClient::~CopySenderClient(){
    qDebug()<<"DELETING(CopySenderClient)";
    if(socket != 0){
        socket->disconnectFromHost();
        socket->deleteLater();
    }
    differentBuilds.clear();
}

QString CopySenderClient::startJSONMessage(){
    return QString("{");
}

void CopySenderClient::appendJSONValue(QString &currentString, QString newKey, QString newValue, bool addComma){
    QString appendThis = "\""+newKey+"\" : \""+newValue+"\"";
    if(addComma)
        appendThis.append(",");
    currentString.append(appendThis);
}


void CopySenderClient::endJSONMessage(QString &currentString){
    currentString.prepend("||");
    currentString.append("}");
    currentString.append("||");
}

void CopySenderClient::disconnectedFunction(){
    this->deleteLater();
}

QString CopySenderClient::getMachineID(){
    int machineID = -1;
    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("Connection");

    //default settings values
    QVariant defaultMachineId;
    defaultMachineId.setValue(-1);

    //send in the default values in case it does not exist...
    QString loadedPort = setting.value("machineID", defaultMachineId).toString();

    setting.endGroup();

    bool validID;
    int theLoadedId = loadedPort.toInt(&validID);

    if(!validID){
        machineID = -1;
    }else{
        if(theLoadedId >= 0)
            machineID = theLoadedId;
        else{
            machineID = -1;
        }
    }

    qDebug()<<"MACHINEID::"<<machineID;
    return QString::number(machineID);
}

bool CopySenderClient::connectToHost(){
    //hostAddress.setAddress();
    socket->connectToHost(hostAddress, port);

    //wait for one second for connection
    if(!socket->waitForConnected(1000)){
        //error
        qDebug()<<"Error when connecting";
        return false;
    }

    QString machineID = getMachineID();

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "HelloCopySender", true);
    appendJSONValue(jsonMessage, "machineID", machineID, false);
    endJSONMessage(jsonMessage);

    socket->write(jsonMessage.toAscii().data());
    socket->flush();

    return true;
}

void CopySenderClient::readyReadFunction(){
    if(socket==0)
        return;
    QByteArray Data = socket->readAll();
    QString data = Data;
    handle(data);
}

void CopySenderClient::handle(QString data){

    QStringList protocolRequests;

    //firstly strip the slashes to reveal all the requests sent through(in case of multiple requests)
    while(data.contains("|")){
        int indexOfLine = -1;
        int startRequestIndex = 0;
        for( ; startRequestIndex < data.length(); startRequestIndex++){
            //a request begins, find the index of vertical line
            if(data.at(startRequestIndex) != '|'){
                indexOfLine = data.indexOf("|", startRequestIndex);
                break;//stop finding next vertical lines
            }
        }

        if(indexOfLine == -1)
            break;//stop if the indexOfLine is not set

        QString newRequest = data.mid(startRequestIndex, (indexOfLine - startRequestIndex));

        protocolRequests.append(newRequest);

        //remove the current request + the lines
        data.remove(0, (indexOfLine + startRequestIndex));
    }

    ///////////HANDLE ALL THE REQUESTS THAT IS SENT THROUGH///////////////
    /////////////E.g. It could be multiple requests///////////////////
    for(int i = 0; i < protocolRequests.length(); i++){
        requestHandler(protocolRequests.at(i));
    }
}


void CopySenderClient::requestHandler(QString data){

    qDebug()<<"copuRead::"<<data;
    const QVariantMap jsonObject = JSON::instance().parse(data);
    QVariant handler = jsonObject.value("handler");

    if(!handler.toString().compare("QVariant(, )")){
        qDebug()<< "invalid JSON String::"<<data;
        return;
    }

    if(firstTalk){
        if(!handler.toString().compare("HelloCopyReceiver")){
            firstTalk = false;
        }else{
            socket->disconnectFromHost();
            return;
        }

        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "SendDifferences",false);
        endJSONMessage(jsonMessage);


        qDebug()<<jsonMessage;
        socket->write(jsonMessage.toAscii().data());
        socket->flush();
        return;
    }


    //else it is not the first time communicating...
    if(!handler.toString().compare("BuildDifferent"))
        BuildDifferent(jsonObject);
    else

    if(!handler.toString().compare("EndAllDifferences"))
        EndAllDifferences();
    else

    if(!handler.toString().compare("DeleteFilesList"))
        DeleteFilesList(jsonObject);

}

void CopySenderClient::BuildDifferent(QVariantMap jsonObject){

    QString differentBuildNo =  jsonObject.value("differentBuildNo").toString();

    differentBuilds.append(differentBuildNo);


}

void CopySenderClient::EndAllDifferences(){

    for(int i = 0; i < differentBuilds.size(); i++){
        QString aBuildDir = allBuildsDirectory + "/" + differentBuilds.at(i);
        BuildMD5 *md5Class = getBuildMD5Class(aBuildDir);
        SendBuildMD5Class(md5Class, i);
        md5Class = 0;
    }


    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage,"handler", "DoneMD5AllFiles", false);
    endJSONMessage(jsonMessage);


    socket->write(jsonMessage.toAscii().data());
    socket->flush();
}

BuildMD5* CopySenderClient::getBuildMD5Class(QString directory){
    BuildMD5* md5class = new BuildMD5();
    md5class->generateAllMD5(directory);
    return md5class;
}


void CopySenderClient::SendBuildMD5Class(BuildMD5 *md5Class, int i){

    //What this function does is create a json string in the following format
    //and writes it to the socket:
    /*

     {
        "handler":"BuildFileSumMD5",
        "buildNo":"[No of the build]",
        "BuildToMD5": {
                        { "[FilePath1]" : "[FileMD5Value1]" },
                        { "[FilePath2]" : "[FileMD5Value1]" },
                        ...........(n Times),
                        { "[FilePathn]" : "[FileMD5Value1]" }
                       }
     }

     where n = the number of
     */

    QString jsonString = "{";
    jsonString.append("\"handler\" : \"BuildFileSumMD5\", \"buildNo\" : \""+differentBuilds.at(i)+"\", \"BuildToMD5\" : {");

    while(md5Class->getCurrentIndex() < md5Class->getSize()){
        const QString *buildFileMd5sum = md5Class->getCurrentFileMd5Sum();
        const QString *buildFileDirectory = md5Class->getCurrentBuildDirectory();
        if(buildFileDirectory == NULL || buildFileMd5sum == NULL )
            break;
        //add to the array of buildMD5 the md5values
        int begin = QString(allBuildsDirectory + "/" + differentBuilds.at(i)).length()+1;
        int amount = buildFileDirectory->length() - QString(allBuildsDirectory + "/" + differentBuilds.at(i)).length()+1;
        jsonString.append("\""+(*buildFileDirectory).mid(begin, amount)+"\" : \""+*buildFileMd5sum+"\"");
        md5Class->next();
        if(md5Class->getCurrentIndex()<md5Class->getSize())
            jsonString.append(",");
    }
    //finally add the end tag of the md5sum array
    jsonString.append("} }");

    jsonString = "||" + jsonString +"||";
    socket->write(jsonString.toAscii().data());
    socket->flush();

    md5Class->deleteLater();
}

void CopySenderClient::DeleteFilesList(const QVariantMap jsonObject){
    QVariant filePaths = jsonObject.value("filePaths");

    QVariant buildNo = jsonObject.value("buildNo");

    QVariantList deleteList = filePaths.toList();

    for(int i = 0; i < deleteList.size(); i++){
        QVariantMap mappingToFile = deleteList.at(i).toMap();
        QString deleteFilePath = mappingToFile.value(mappingToFile.keys().at(0)).toString();
        deleteFilePath = allBuildsDirectory+ "/" + buildNo.toString() + "/" + deleteFilePath;
        QFile::remove(deleteFilePath);

        int indexOfLastSlash = -1;
        for(int i = 0; i < deleteFilePath.length(); i++){
            if(deleteFilePath.at(i) == '/'){
                indexOfLastSlash = i;
            }
        }
        QString directoryWhereDeleted = deleteFilePath.left(indexOfLastSlash);

        if(indexOfLastSlash != -1){//if it was -1 it means it is root in that build
            //check to see if the directory where file is removed from is empty, if so delete the directory as well
            if( ( QDir(directoryWhereDeleted).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0 ) & ( !directoryWhereDeleted.compare(buildNo.toString()) ) ){
                QDir().rmdir(directoryWhereDeleted);
            }
        }
    }
}
