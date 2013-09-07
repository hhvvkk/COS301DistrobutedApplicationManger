#include "copysenderserver.h"

CopySenderServer::CopySenderServer( QStringList &differentB, QStringList &differentBNos, Management *man, QObject *parent) :
    QTcpServer(parent)
{
    differentBuildDirectories = differentB;
    differentBuildNos = differentBNos;
    management = man;

    qDebug()<<"All the different Builds follows::in CopySenderServer";
    for(int i = 0; i < differentBuildDirectories.size(); i++){
        qDebug()<<"DifferentBuild="<<differentBuildDirectories.at(i);
    }

    firstTalk = true;
}

CopySenderServer::~CopySenderServer(){
    qDebug()<<"DELETING";
    if(socket != 0)
        socket->deleteLater();
}

int CopySenderServer::startServer(){
    if(!this->listen(QHostAddress::Any)){
        qDebug() << "Could not start server";
    }
    else
        qDebug() << "SenderServerListening...";

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

    qDebug()<<"CopyreceiverConnected="<<socket->peerAddress().toString();
    if(!socket){
        qDebug()<<"Unable to send files";
        destroyServer();
        return;
    }

    connect(socket, SIGNAL(disconnected()), this, SLOT(destroyServer()));
    connect(socket, SIGNAL(readyRead()),this,SLOT(readyReadFunction()));
}

void CopySenderServer::destroyServer(){
    qDebug()<<"DESTROYING SERVER";
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


    ///////////HANDLE ALL THE REQUESTS THAT IS SENT THROUGH///////////////
    /////////////E.g. It could be multiple requests///////////////////
    for(int i = 0; i < protocolRequests.length(); i++){
        requestHandler(protocolRequests.at(i));
    }

}


void CopySenderServer::requestHandler(QString data){
    if(data.at(0) == '{'){
        const QVariantMap jsonObject = JSON::instance().parse(data);

        QVariant handler = jsonObject.value("handler");
        if(!handler.toString().compare("BuildFileSumMD5"))
            getDifferences(jsonObject);
       // else if(!handler.toString().compare("BuildFileSumMD5"))

    }

    if(firstTalk){
        qDebug()<<"firsttalk"<<data;
        if(!data.compare("HelloCopySender")){
            firstTalk = false;
            //stop the server since you don't want to allow any other connections
            stopServer();
        }else{
            socket->disconnectFromHost();
            return;
        }
        socket->write("||HelloCopyReceiver||");
        socket->flush();
        return;
    }

    if(!data.compare("SendDifferences")){
        SendDifferences();
    }
}


void CopySenderServer::SendDifferences(){
    //loop through all the directories in order to state which is different
    //on the client
    for(int i = 0; i < differentBuildNos.size(); i++){
        //if it is the first time it is connected write to the client which builds
        //should generate the MD5classes for
        QString differentBuildMessage = "||BuildDifferent:#"+differentBuildNos.at(i)+"||";
        socket->write(differentBuildMessage.toAscii().data());
        socket->flush();
    }
    socket->write("||EndAllDifferences||");
    socket->flush();
}

void CopySenderServer::getDifferences(const QVariantMap jsonObject){


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
    management->machineBuildSynched(ipAddress, copyCompareForBuild->percentageSynched());
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
