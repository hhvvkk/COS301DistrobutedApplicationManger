#include "copysenderclient.h"
#include "directoryhandler.h"


CopySenderClient::CopySenderClient(QHostAddress hAdr, int portNumber, QObject *parent) :
    QObject(parent),
    port(portNumber),
    hostAddress(hAdr)
{
    copyList = 0;
    socket = new QTcpSocket();
    firstTalk = true;
    //connect(socket, SIGNAL(connected()), socketClient, SLOT(connected()));
    allBuildsDirectory = "builds";

    copyList = new QList<CopierPhysicalClient*>();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadFunction()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedFunction()));
}

CopySenderClient::~CopySenderClient(){
    CopierPhysicalClient* cpPhy;
    if(copyList != 0){
        if(copyList->size() != 0){
            while(copyList->size() != 0){
                cpPhy = copyList->at(0);
                copyList->removeOne(cpPhy);
                cpPhy->deleteLater();
            }
        }
    }


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
    return QString::number(machineID);
}

bool CopySenderClient::connectToHost(){
    //hostAddress.setAddress();
    socket->connectToHost(hostAddress, port);

    //wait for one second for connection
    if(!socket->waitForConnected(1000)){
        //error
        return false;
    }

    QString machineID = getMachineID();

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "HelloCopySender", true);
    appendJSONValue(jsonMessage, "machineID", machineID, false);
    endJSONMessage(jsonMessage);

    socket->write(jsonMessage.toUtf8().data());
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


        socket->write(jsonMessage.toUtf8().data());
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
    else

    if(!handler.toString().compare("ConnectPhysicalServer"))
        ConnectPhysicalServer(jsonObject);
    else

    if(!handler.toString().compare("PhysicalServerDone"))
        PhysicalServerDone(jsonObject);

}

void CopySenderClient::BuildDifferent(QVariantMap jsonObject){

    QString differentBuildID =  jsonObject.value("differentBuildID").toString();

    differentBuilds.append(differentBuildID);


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


    socket->write(jsonMessage.toUtf8().data());
    socket->flush();
}

BuildMD5* CopySenderClient::getBuildMD5Class(QString directory){
    BuildMD5* md5class = new BuildMD5(directory,5);
    md5class->generate();
    return md5class;
}


void CopySenderClient::SendBuildMD5Class(BuildMD5 *md5Class, int i){

    //What this function does is create a json string in the following format
    //and writes it to the socket:
    /*

     {
        "handler":"BuildFileSumMD5",
        "BuildID":"[No of the build]",
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
    jsonString.append("\"handler\" : \"BuildFileSumMD5\", \"BuildID\" : \""+differentBuilds.at(i)+"\", \"BuildToMD5\" : {");

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
    socket->write(jsonString.toUtf8().data());
    socket->flush();

    md5Class->deleteLater();
}

void CopySenderClient::DeleteFilesList(const QVariantMap jsonObject){

    QVariant filePaths = jsonObject.value("filePaths");

    QVariant BuildID = jsonObject.value("BuildID");

    QVariantList deleteList = filePaths.toList();

    for(int i = 0; i < deleteList.size(); i++){
        QVariantMap mappingToFile = deleteList.at(i).toMap();
        QString deleteFilePath = mappingToFile.value(mappingToFile.keys().at(0)).toString();
        deleteFilePath = allBuildsDirectory+ "/" + BuildID.toString() + "/" + deleteFilePath;
        if(QFile(deleteFilePath).exists()){
            QFile().remove(deleteFilePath);
        }
        if(QDir(deleteFilePath).exists()){
            //QDir().remove(deleteFilePath);
            //bring in the big guns(Directory handler) to delete the directory
            DirectoryHandler::removeDir(deleteFilePath);
            continue;
        }

        int indexOfLastSlash = -1;
        for(int i = 0; i < deleteFilePath.length(); i++){
            if(deleteFilePath.at(i) == '/'){
                indexOfLastSlash = i;
            }
        }
        QString directoryWhereDeleted = deleteFilePath.left(indexOfLastSlash);

        if(indexOfLastSlash != -1){//if it was -1 it means it is root in that build
            //check to see if the directory where file is removed from is empty, if so delete the directory as well
            if( ( QDir(directoryWhereDeleted).entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0 ) & ( !directoryWhereDeleted.compare(BuildID.toString()) ) ){
                DirectoryHandler::removeDir(directoryWhereDeleted);
            }
        }
    }
}

void CopySenderClient::ConnectPhysicalServer(const QVariantMap jsonObject){
    int port = jsonObject.value("port").toInt();
    if(port <= 0){
        return;
    }
    bool ok = false;
    int BuildID = jsonObject.value("BuildID").toInt(&ok);
    if(!ok)
        return;

    //This magic number depends on how the build is compressed on the master machine
    //and will determine how the files are found and extracted to the correct directory
    int magicNumber = jsonObject.value("magicNumber").toInt(&ok);
    if(!ok)
        return;

    //connect to that server...
    CopierPhysicalClient * physicalClient = new CopierPhysicalClient(hostAddress, port, BuildID, magicNumber);
    connect(physicalClient, SIGNAL(doneWritingToFile(int, bool)), this, SLOT(doneWritingToFile(int, bool)) );

    bool connected = physicalClient->connectToHost();

    if(!connected){
        physicalClient->deleteLater();
        return;
    }//else let it delete itself after it is done...


    copyList->append(physicalClient);
}

void CopySenderClient::PhysicalServerDone(const QVariantMap jsonObject){
    bool ok;
    int BuildID = jsonObject.value("BuildID").toInt(&ok);

    if(!ok)
        return;


    lock.lock();
    CopierPhysicalClient *cpPhysical = 0;
    //find the copier that is done

    for(int i = 0; i < copyList->size(); i++){
        if(copyList->at(i)->getBuildID() == BuildID){
            cpPhysical = copyList->at(i);
            break;
        }
    }
    lock.unlock();

    if(cpPhysical == 0){
        qDebug()<<"not found...";
        return;
    }

    //let the copier write to the file it is supposed to write to...
    cpPhysical->writeToFile();

    copyList->removeOne(cpPhysical);
    cpPhysical->deleteLater();

}


void CopySenderClient::doneWritingToFile(int BuildID, bool success){
    //this time it is done writing all the necessary information to the zip file

    //find the copierPhysicalClient which emitted the signal
    lock.lock();
    CopierPhysicalClient *cpPhysical = 0;
    for(int i = 0; i < copyList->size(); i++){
        if(copyList->at(i)->getBuildID() == BuildID){
            cpPhysical = copyList->at(i);
            break;
        }
    }
    lock.unlock();


    if(cpPhysical == 0){
        return;
    }

    if(!success){
        //if it was not successful...create another request to send that build...(inside notify)
        cpPhysical->removeZipFile();
    }

    //if the zip file was sucessfully sent across, only remove...
    bool successfulRemove = copyList->removeOne(cpPhysical);
    if(!successfulRemove){
        qDebug()<<"not successful remove--copysenderlclient--line401";
    }

    //delete the physical copier
    cpPhysical->deleteLater();

    //notify if it has been a success or not...

    notifyServerSuccess(BuildID, success);
}

void CopySenderClient::notifyServerSuccess(int BuildID, bool success){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "NotifyCopySuccess", true);
    appendJSONValue(jsonMessage, "BuildID", QString::number(BuildID), true);
    appendJSONValue(jsonMessage, "success", QString::number(success), false);
    endJSONMessage(jsonMessage);

    socket->write(jsonMessage.toUtf8().data());
    socket->flush();
}
