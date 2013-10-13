#include "copysenderserver.h"

CopySenderServer::CopySenderServer( QStringList *differentB, QStringList *differentBNos, Management *man, int mashId, QObject *parent) :
    QTcpServer(parent)
{
    differentBuildDirectories = differentB;
    differentBuildIDs = differentBNos;
    management = man;
    machineId = mashId;

    firstTalk = true;
    QtConcurrent::run(this, &CopySenderServer::loadCompressPath);

    //the copier queue will be created at another location
    copierQueue = 0;
    socket = 0;
    zipFilesToDelete = false;
}

CopySenderServer::~CopySenderServer(){
    if(socket != 0){
        socket->disconnectFromHost();
        socket->deleteLater();
    }
    if(copierQueue != 0)
        copierQueue->deleteLater();
    if(differentBuildDirectories != 0){
        differentBuildDirectories->clear();
        delete differentBuildDirectories;
    }
    if(differentBuildIDs != 0){
        differentBuildIDs->clear();
        delete differentBuildIDs;
    }
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
        //firstly try and create that directory...
        bool successCreate = QDir().mkdir(loadedFileCompressPath);
        //...if it fails, revert to default
        if(!successCreate){
            fileCompressPath = "buildCompressed";
            QDir().mkdir(fileCompressPath);
        }
    }
    else{
        fileCompressPath = loadedFileCompressPath;
    }
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


void CopySenderServer::sendJSONMessage(QTcpSocket *slaveSocket, QString jsonMessage){
    if(slaveSocket == 0){
        return;
    }

    try{
        slaveSocket->write(jsonMessage.toAscii().data());
        slaveSocket->flush();
    }catch(...){
        //if it fails to write and throws an exception somehow, you catch the exception
    }
}

int CopySenderServer::startServer(){
    if(!this->listen(QHostAddress::Any)){
        qDebug() << "Could not start server";
    }

//    else
//        qDebug() << "SenderServerListening...";

    //returns a zero if it is not listening otherwise a server port
    return this->serverPort();
}


void CopySenderServer::stopServer(){
    this->close();
}


bool CopySenderServer::isBusyDeleting(){
    return zipFilesToDelete;
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

    QFuture <QStringList>future = QtConcurrent::run(this, &CopySenderServer::splitRequests, data);

    QStringList protocolRequests = future.result();

    if(protocolRequests.isEmpty())
        return;//no requests found

    ///////////HANDLE ALL THE REQUESTS THAT IS SENT THROUGH///////////////
    /////////////E.g. It could be multiple requests///////////////////
    for(int i = 0; i < protocolRequests.length(); i++){
        requestHandler(protocolRequests.at(i));
    }

}


QStringList CopySenderServer::splitRequests(QString data){
    QStringList requests;
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

        requests.append(newRequest);

        //remove the current request + the lines
        data.remove(0, (indexOfLine + startRequestIndex));
    }

    return requests;
}


void CopySenderServer::requestHandler(QString data){

    JSON *instance = &JSON::instance();

    //go and parse the json to an object concurrently...
    QFuture <QVariantMap>future = QtConcurrent::run(instance, &JSON::parse, data);

    const QVariantMap jsonObject = future.result();//finally retrieve the jsonObject after it has been parsed
    QVariant handler = jsonObject.value("handler");

    if(!handler.toString().compare("QVariant(, )")){
        qDebug()<< "invalid JSON String::"<<data;
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


        sendJSONMessage(socket, jsonMessage);
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
    for(int i = 0; i < differentBuildIDs->size(); i++){
        //if it is the first time it is connected write to the client which builds
        //should generate the MD5classes for
        QString jsonMessage = startJSONMessage();
        appendJSONValue(jsonMessage, "handler", "BuildDifferent", true);
        appendJSONValue(jsonMessage, "differentBuildID", differentBuildIDs->at(i),false);
        endJSONMessage(jsonMessage);

        sendJSONMessage(socket, jsonMessage);
    }

    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler", "EndAllDifferences", false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(socket, jsonMessage);
}

void CopySenderServer::BuildFileSumMD5(const QVariantMap jsonObject){
    QVariant allMD5s = jsonObject.value("BuildToMD5");
    QVariant BuildID = jsonObject.value("BuildID");

    //generate the build md5 class for that build
    QString theBuildDirectory = "";
    for(int i = 0; i < differentBuildDirectories->size(); i++){
        if(!BuildID.toString().compare(differentBuildIDs->at(i))){
            theBuildDirectory = differentBuildDirectories->at(i);
            break;
        }
    }

    if(!theBuildDirectory.compare(""))
        return;


    BuildMD5 *buildMD5Class = new BuildMD5(theBuildDirectory,5);

    QFuture <void>future = QtConcurrent::run(buildMD5Class, &BuildMD5::generate);
    future.waitForFinished();//wait untill the md5 class has been created

    //here it gets all the build md5 values with their keys(The keys are their file directory)
    QVariantMap mapOfBuilds = allMD5s.toMap();
    QList<QString> keys = mapOfBuilds.keys();

    if(keys.size() == 0)//if map is empty(end it)
        return;

    //create a copy compare class to be able to build it later on
    //make use of QConcurrent to do so
    QFuture <CopyCompare *> futureCopyCompare = QtConcurrent::run(this, &CopySenderServer::createCopyCompare, keys, mapOfBuilds, buildMD5Class, theBuildDirectory);
    //after computation retrieve the future value
    CopyCompare *copyCompareForBuild = futureCopyCompare.result();

    //done with the class so delete it
    buildMD5Class->deleteLater();
    buildMD5Class = 0;

    QString deleteJsonString = copyCompareForBuild->getDeleteJsonString(BuildID.toString());

    if(!deleteJsonString.isEmpty()){
        socket->write(("||"+deleteJsonString+"||").toAscii().data());
        socket->flush();
    }

    //do something with that
    //     void machineBuildSynched(int machineId, int buildId, double percentageSynched);
    QtConcurrent::run(management, &Management::machineBuildSynched, machineId,BuildID.toInt(), copyCompareForBuild->percentageSynched());

    Compression c;
    QFuture <void>compressFuture = QtConcurrent::run(&c, &Compression::compress,copyCompareForBuild->getFilepaths(),  fileCompressPath+ "/" + QString::number(machineId) + "/"+BuildID.toString(), theBuildDirectory);

    //wait untill it has finished compressing
    compressFuture.waitForFinished();
    // c.compress(copyCompareForBuild->getFilepaths(), fileCompressPath+ "/" + QString::number(machineId) + "/"+BuildID.toString(), theBuildDirectory);

    int intBuildID = BuildID.toInt();
    createPhysicalCopier(intBuildID);

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

void CopySenderServer::PhysicalServerDoneNotify(int BuildID){
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler","PhysicalServerDone", true);
    appendJSONValue(jsonMessage, "BuildID", QString::number(BuildID),false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(socket, jsonMessage);
}


void CopySenderServer::createPhysicalCopier(int BuildID){
    QString zipDirectory = fileCompressPath + "/" + QString::number(machineId) + "/" + QString::number(BuildID) + ".7z";

    CopierPhysical *physicalCopier = new CopierPhysical(machineId, BuildID, zipDirectory);

    //connect the signal to be able to notify the physical copier when the copying has been completed
    connect(physicalCopier, SIGNAL(copierPhysicalDone(int)), this, SLOT(PhysicalServerDoneNotify(int)));
    connect(physicalCopier, SIGNAL(notifyProgress(int,int,int)), this, SLOT(notifyProgress(int,int,int)));

    addToQueue(physicalCopier);
}

void CopySenderServer::addToQueue(CopierPhysical *physicalCopier){
    //this lock is to prevent the event of a copyQueue that is NULL
    lock.lock();
    //firstly check if the queue is ready to accept a new physicalCopier(if not it is deleting)
    //firstly check for NULL
    if(copierQueue == 0){
        copierQueue = new CopyQueue();
        connect(copierQueue, SIGNAL(queueFinished(CopyQueue*)), this, SLOT(queueFinished(CopyQueue*)));
        connect(copierQueue, SIGNAL(nextInQueue(int,int)), this, SLOT(nextInQueue(int,int)));
    }

    //if it is ready, attempt to add to the queue
    bool successfulAdd = copierQueue->append(physicalCopier);

    //if it is unsuccessfull it means that the queue is about to delete,
    //and it is required to add a new copyQueue. The reference will go and delete
    //itself at the queueFinished(copyQ*) function
    if(!successfulAdd){
        copierQueue = new CopyQueue();
        copierQueue->append(physicalCopier);
        connect(copierQueue, SIGNAL(queueFinished(CopyQueue*)), this, SLOT(queueFinished(CopyQueue*)));
        connect(copierQueue, SIGNAL(nextInQueue(int,int)), this, SLOT(nextInQueue(int,int)));
    }

    copierQueue->startCopying();

    lock.unlock();
}

void CopySenderServer::NotifyCopySuccess(const QVariantMap jsonObject){
    //appendJSONValue(jsonMessage, "BuildID", QString::number(BuildID), true);
    //appendJSONValue(jsonMessage, "success", QString::number(success), false);

    bool success = jsonObject.value("success").toBool();
    int BuildID = jsonObject.value("BuildID").toInt();


    //if it was a success just delete the zip(it is done copying it over...)
    if(success){
        zipFilesToDelete = true;
        QString zipDirectory = fileCompressPath + "/"+ QString::number(machineId) + "/" + QString::number(BuildID) + ".7z";
        QFile zipFile(zipDirectory);
        zipFile.remove();
        zipFile.waitForBytesWritten(-1);
        zipFilesToDelete = false;
    }
    //otherwise recreate a physical copier to resend the zip file
    else{
        createPhysicalCopier(BuildID);
    }
}

//notifyProgress(i, buffer.size(), BuildID);
void CopySenderServer::notifyProgress(int index, int bufferSize, int BuildID){
    //Management::machineBuildSynched(int machineId, int buildId, double percentageSynched)
    //double progress =
   // qDebug()<<"int index = "<<index;
}


void CopySenderServer::nextInQueue(int port, int BuildID){
    //this will go and notify the copysenderclient about a new server awaiting a connection
    QString jsonMessage = startJSONMessage();
    appendJSONValue(jsonMessage, "handler","ConnectPhysicalServer",true);
    appendJSONValue(jsonMessage, "BuildID", QString::number(BuildID), true);
    appendJSONValue(jsonMessage, "port", QString::number(port), false);
    endJSONMessage(jsonMessage);

    sendJSONMessage(socket, jsonMessage);
}

void CopySenderServer::queueFinished(CopyQueue * theCopyQueue){
    //this lock is to prevent the event of a copyQueue that is not empty and want to delete
    lock.lock();
    if(theCopyQueue == copierQueue){
        theCopyQueue->deleteLater();
        //remove dangling pointer for other calls
        copierQueue = 0;
        emit copySenderServerDone(this);
    }
    else{
        //in this instance the copierQueue has already been changed at
        //the addToQueue function
        theCopyQueue->deleteLater();
    }
    lock.unlock();
}



