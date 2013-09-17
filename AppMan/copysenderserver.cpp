#include "copysenderserver.h"
#include "copierphysical.h"

CopySenderServer::CopySenderServer( QStringList &differentB, QStringList &differentBNos, Management *man, int mashId, QObject *parent) :
    QTcpServer(parent)
{
    differentBuildDirectories = differentB;
    differentBuildNos = differentBNos;
    management = man;
    machineId = mashId;

    firstTalk = true;
    loadCompressPath();
}

void CopySenderServer::loadCompressPath(){
    QSettings setting("settings.ini",QSettings::IniFormat);
    //grouping the settings
    setting.beginGroup("FileCopy");

    //default settings values
    QVariant defaultfileCompressPath;
    QString theDefaultPathCompress = "buildCompressed";
    defaultfileCompressPath.setValue(theDefaultPathCompress);

    //send in the default values in case it does not exist...
    QString loadedFileCompressPath = setting.value("fileCompressPath", defaultfileCompressPath).toString();

    setting.endGroup();

    if(!QDir(loadedFileCompressPath).exists()){
        fileCompressPath = "buildCompressed";
    }
    else{
        fileCompressPath = loadedFileCompressPath;
    }
}

CopySenderServer::~CopySenderServer(){
    qDebug()<<"DELETING";
    if(socket != 0)
        socket->deleteLater();
}


QString CopySenderServer::startJSONMessage(){
    return QString("{");
}

void CopySenderServer::appendJSONValue(QString &currentString, QString newKey, QString newValue, bool addComma){
    QString appendThis = "\""+newKey+"\" : \""+newValue+"\"";
    if(addComma)
        appendThis.append(",");
    currentString.append(appendThis);
}



void CopySenderServer::endJSONMessage(QString &currentString){
    currentString.prepend("||");
    currentString.append("}");
    currentString.append("||");
}


int CopySenderServer::startServer(){
    if(!this->listen(QHostAddress::Any)){
        //qDebug() << "Could not start server";
    }
//    else
//        qDebug() << "SenderServerListening...";

    //returns a zero if it is not listening otherwise a server port
    return this->serverPort();
}


void CopySenderServer::stopServer(){
    this->close();
}

void CopySenderServer::incomingConnection(int socketID){
    socket = new QTcpSocket();

    //set the socket descriptor to that client which connected
    socket->setSocketDescriptor(socketID);


    if(!socket){
        qDebug()<<"Unable to send files";
        destroyServer();
        return;
    }

    connect(socket, SIGNAL(disconnected()), this, SLOT(destroyServer()));
    connect(socket, SIGNAL(readyRead()),this,SLOT(readyReadFunction()));
}

void CopySenderServer::destroyServer(){
    this->deleteLater();
}

void CopySenderServer::readyReadFunction(){
    QByteArray Data = socket->readAll();
    QString data = Data;

    handle(data);
}

void CopySenderServer::handle(QString data){
    if(socket == 0){
        qDebug()<<"socket = 0(handle--CopySenderServer)";
        return;
    }

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


    ///////////HANDLE ALL THE REQUloadCompressPathESTS THAT IS SENT THROUGH///////////////
    /////////////E.g. It could be multiple requests///////////////////
    for(int i = 0; i < protocolRequests.length(); i++){
        requestHandler(protocolRequests.at(i));
    }

}


void CopySenderServer::requestHandler(QString data){
    const QVariantMap jsonObject = JSON::instance().parse(data);
    QVariant handler = jsonObject.value("handler");

    if(!handler.toString().compare("QVariant(, )")){
        qDebug()<< "invalid JSON String::"<<data;
        return;
    }

    if(firstTalk){
        if(!handler.toString().compare("HelloCopySender")){
            //check whether the machine is the right machine that should connect to it...
            if(jsonObject.value("machineID").toString().compare(QString::number(machineId))){
               socket->disconnectFromHost();
               return;
            }else{
                //if it is the right machine continue
                firstTalk = false;
                //stop the server since you don't want to allow any other connections
                stopServer();
            }
        }else{
            socket->disconnectFromHost();
            return;
        }


        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "HelloCopyReceiver", false);
        endJSONMessage(jsonMessage);


        socket->write(jsonMessage.toAscii().data());
        socket->flush();
        return;
    }


    if(!handler.toString().compare("SendDifferences"))
        SendDifferences();
    else

    if(!handler.toString().compare("BuildFileSumMD5"))
        BuildFileSumMD5(jsonObject);
    else

    if(!handler.toString().compare("NotifyCopySuccess"))
        NotifyCopySuccess(jsonObject);

}


void CopySenderServer::SendDifferences(){
    //loop through all the directories in order to state which is different
    //on the client
    for(int i = 0; i < differentBuildNos.size(); i++){
        //if it is the first time it is connected write to the client which builds
        //should generate the MD5classes for
        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "BuildDifferent", true);
        appendJSONValue(jsonMessage, "differentBuildNo",differentBuildNos.at(i),false);
        endJSONMessage(jsonMessage);

        socket->write(jsonMessage.toAscii().data());
        socket->flush();
    }

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "EndAllDifferences", false);
    endJSONMessage(jsonMessage);

    socket->write(jsonMessage.toAscii().data());
    socket->flush();
}

void CopySenderServer::BuildFileSumMD5(const QVariantMap jsonObject){

    QVariant allMD5s = jsonObject.value("BuildToMD5");
    QVariant buildNo = jsonObject.value("buildNo");

    //generate the build md5 class for that build
    QString theBuildDirectory = "";
    for(int i = 0; i < differentBuildDirectories.size(); i++){
        if(!buildNo.toString().compare(differentBuildNos.at(i))){
            theBuildDirectory = differentBuildDirectories.at(i);
            break;
        }
    }

    if(!theBuildDirectory.compare(""))
        return;

    BuildMD5 *buildMD5Class = new BuildMD5();
    buildMD5Class->generateAllMD5(theBuildDirectory);

    //here it gets all the build md5 values with their keys(The keys are their file directory)
    QVariantMap mapOfBuilds = allMD5s.toMap();
    QList<QString> keys = mapOfBuilds.keys();

    //create a copy compare class to be able to build it later on
    CopyCompare *copyCompareForBuild = createCopyCompare(keys, mapOfBuilds, buildMD5Class, theBuildDirectory);

    //done with the class so delete it
    buildMD5Class->deleteLater();
    buildMD5Class = 0;

    QString deleteJsonString = copyCompareForBuild->getDeleteJsonString(buildNo.toString());

    if(!deleteJsonString.isEmpty()){
        socket->write(("||"+deleteJsonString+"||").toAscii().data());
        socket->flush();
    }

    //do something with that
    //     void machineBuildSynched(int machineId, int buildId, double percentageSynched);
    management->machineBuildSynched(machineId, buildNo.toInt(),copyCompareForBuild->percentageSynched());

    Compression c;
    c.compress(copyCompareForBuild->getFilepaths(), fileCompressPath+ "/" + QString::number(machineId) + "/"+buildNo.toString(), theBuildDirectory);


    int intBuildNo = buildNo.toInt();
    createPhysicalCopier(intBuildNo);

    delete copyCompareForBuild;
}

CopyCompare *CopySenderServer::createCopyCompare(QList<QString> &keys, QVariantMap mapOfBuilds, BuildMD5 *md5Class, QString theBuildDirectory)  const{
    CopyCompare * copyCompare = new CopyCompare();

    //the following is to determine all the files that will be deleted
    QList<int> comparedFiles;

    //this from is to reduce time...
    //The from will increase as the index is reduced
    int from = 0;

    while(md5Class->getCurrentIndex() < md5Class->getSize()){
        QString fileToFind = *md5Class->getCurrentBuildDirectory();
        fileToFind = fileToFind.mid(theBuildDirectory.length()+1);
        int indexOfKey = keys.indexOf(fileToFind, from);
        if(indexOfKey != -1){//this means that there exist a key such that the build directory exist
            //the following has the potential to reduce the time it would take to find the index of the key
            if(from == indexOfKey){
                from++;
            }
            copyCompare->compareMD5(md5Class->getCurrentFileMd5Sum(), md5Class->getCurrentBuildDirectory(), mapOfBuilds.value(keys.at(indexOfKey)).toString());
            comparedFiles.append(indexOfKey);
        }else{
            //that does not exist in the slave machine
            copyCompare->createFile(*md5Class->getCurrentBuildDirectory());
        }
        //go to the next file
        md5Class->next();
    }

    //all the files that will be deleted...
    for(long i = 0; i < keys.size(); i++){
        //append all the files that
        int index = comparedFiles.indexOf(i);
        //if it does not find the file within the list of compared files...
        //then it must be deleted as it does not exist on the master anymore
        if(index <= -1){
            copyCompare->deleteFile(keys.at(i));
        }
    }

    return copyCompare;
}

void CopySenderServer::PhysicalServerDoneNotify(int buildNo){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler","PhysicalServerDone", true);
    appendJSONValue(jsonMessage, "buildNo", QString::number(buildNo),false);
    endJSONMessage(jsonMessage);

    socket->write(jsonMessage.toAscii().data());
    socket->flush();
}


void CopySenderServer::createPhysicalCopier(int buildNo){
    QString zipDirectory = fileCompressPath + "/" + QString::number(machineId) + "/" + QString::number(buildNo) + ".7z";

    CopierPhysical *physicalCopier = new CopierPhysical(machineId, buildNo, zipDirectory);

    int port = physicalCopier->startServer();

    //connect the signal to be able to notify the physical copier when the copying has been completed
    connect(physicalCopier, SIGNAL(copierPhysicalDone(int)), this, SLOT(PhysicalServerDoneNotify(int)));
    connect(physicalCopier, SIGNAL(notifyProgress(int,int,int)), this, SLOT(notifyProgress(int,int,int)));

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler","ConnectPhysicalServer",true);
    appendJSONValue(jsonMessage, "buildNo", QString::number(buildNo), true);
    appendJSONValue(jsonMessage, "port", QString::number(port), false);
    endJSONMessage(jsonMessage);

    socket->write(jsonMessage.toAscii().data());
    socket->flush();
}

void CopySenderServer::NotifyCopySuccess(const QVariantMap jsonObject){

    //appendJSONValue(jsonMessage, "buildNo", QString::number(buildNo), true);
    //appendJSONValue(jsonMessage, "success", QString::number(success), false);

    bool success = jsonObject.value("success").toBool();
    int buildNo = jsonObject.value("buildNo").toInt();


    //if it was a success just delete the zip(it is done copying it over...)
    if(success){
        QString zipDirectory = fileCompressPath + "/" + QString::number(buildNo) + ".7z";
        QFile zipFile(zipDirectory);
        zipFile.remove();
    }
    //otherwise recreate a physical copier to resend the zip file
    else{
        createPhysicalCopier(buildNo);
    }
}

//notifyProgress(i, buffer.size(), buildNo);
void CopySenderServer::notifyProgress(int index, int bufferSize, int buildNo){
    //Management::machineBuildSynched(int machineId, int buildId, double percentageSynched)
    //double progress =
    qDebug()<<"int index = "<<index;
}
