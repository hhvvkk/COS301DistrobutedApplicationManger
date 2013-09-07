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

void CopySenderClient::disconnectedFunction(){
    this->deleteLater();
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

    QString helloMessageCopySender = "||HelloCopySender||";
    socket->write(helloMessageCopySender.toAscii().data());
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
    if(data.at(0) == '{'){
        const QVariantMap jsonObject = JSON::instance().parse(data);

        QVariant handler = jsonObject.value("handler");
        if(!handler.toString().compare("DeleteFilesList"))
            DeleteFilesList(jsonObject);
    }

    if(firstTalk){
        if(!data.compare("HelloCopyReceiver")){
            firstTalk = false;
        }else{
            socket->disconnectFromHost();
            return;
        }
        socket->write("||SendDifferences||");
        socket->flush();
        return;
    }

    //else it is not the first time communicating
    if(data.contains("BuildDifferent:#")){
        BuildDifferent(data);
    }

    if(!data.compare("EndAllDifferences")){
        //this means go and create all the Md5Classes
        EndAllDifferences();
    }

}

void CopySenderClient::BuildDifferent(QString data){
    //E.g SendBuildCopyServer:#121.110.222.212#4412
    QString mostLeft = "BuildDifferent:#";

    QString differentBuildNo = data.right((data.size()-mostLeft.length()));
    //E.g. differentBuildNo= "4"

    differentBuilds.append(differentBuildNo);


}

void CopySenderClient::EndAllDifferences(){

    for(int i = 0; i < differentBuilds.size(); i++){
        QString aBuildDir = allBuildsDirectory + "/" + differentBuilds.at(i);
        BuildMD5 *md5Class = getBuildMD5Class(aBuildDir);
        SendBuildMD5Class(md5Class, i);
        md5Class = 0;
    }

    socket->write("||DoneMD5AllFiles||");
    socket->flush();
}

BuildMD5* CopySenderClient::getBuildMD5Class(QString directory){
    BuildMD5* md5class = new BuildMD5();
    md5class->generateAllMD5(directory);
    return md5class;
}


void CopySenderClient::SendBuildMD5Class(BuildMD5 *md5Class, int i){

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
