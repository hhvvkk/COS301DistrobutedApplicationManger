#include "copierphysicalclient.h"
#include "directoryhandler.h"

CopierPhysicalClient::CopierPhysicalClient(QHostAddress hAdr, int portNumber, int bNumber,  QObject *parent) :
    QObject(parent),
    port(portNumber),
    hostAddress(hAdr),
    BuildID(bNumber)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadFunction()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedFunction()));
    allBuildsDirectory = "builds";
    compressDirectory = "compress";
    extractDirectory = "extract";

    /*
     Check if the directories exist for the functions they are intended for
     */
    QDir allBDir(allBuildsDirectory);
    if(!allBDir.exists())
        QDir().mkpath(allBuildsDirectory);

    QDir allCDir(compressDirectory);
    if(!allCDir.exists())
        QDir().mkpath(compressDirectory);

    QDir allEDir(extractDirectory);
    if(!allEDir.exists())
        QDir().mkpath(extractDirectory);

    amountRead = 0;
}

CopierPhysicalClient::~CopierPhysicalClient(){
    if(socket != 0){
        socket->disconnectFromHost();
        socket->deleteLater();
    }
}


int CopierPhysicalClient::getBuildID(){
    return BuildID;
}

bool CopierPhysicalClient::connectToHost(){
    //hostAddress.setAddress();
    socket->connectToHost(hostAddress, port);

    //wait for one second for connection
    if(!socket->waitForConnected(1000)){
        //error
        qDebug()<<"Error when connecting";
        return false;
    }

    socket->write("HelloPhysicalSender");
    socket->flush();

    return true;
}


void CopierPhysicalClient::disconnectedFunction(){
    //Do not delete it, it is deleted at the copySenderServer!!!
}

void CopierPhysicalClient::readyReadFunction(){
    //read all from the socket at that stage
    QByteArray Data = socket->readAll();

    //write to the file buffer the data that was read
    fileBuffer.append(Data);
}

void CopierPhysicalClient::writeToFile(){
    //write the full buffer to the zip file...

    QFile zipFile(compressDirectory+"/"+QString::number(BuildID)+".7z");

    if(zipFile.exists()){
        //qDebug()<<"creating zip received over network";
        zipFile.remove();
    }
    if(!zipFile.open(QFile::ReadWrite)){
        qDebug()<<"Cannot open for read"<<zipFile.fileName();
        return;
    }

    //write all the data from the file buffer that was read previously
    zipFile.write(fileBuffer);
    zipFile.close();

    bool zipCopySuccess = zipInTact();


    emit doneWritingToFile(BuildID, zipCopySuccess);

    if(zipCopySuccess){
        extractZipToDirectory();
    }

    //disconnect from the other machine
    socket->disconnectFromHost();
}


void CopierPhysicalClient::removeZipFile(){
    QFile zipFile(compressDirectory+"/"+QString::number(BuildID)+".7z");

    if(zipFile.exists()){
        zipFile.remove();
    }

}

bool CopierPhysicalClient::zipInTact(){
    Compression c;

    QString zipPath(compressDirectory+"/"+QString::number(BuildID)+".7z");
    //determine whether the zip is in tact. Return true if it is, else false;
    bool zipInTact = c.zipInTact(zipPath);

    //if it was in tact, extract it in the right directory after returning
    return zipInTact;
}


void CopierPhysicalClient::extractZipToDirectory(){
    QString zipPath(compressDirectory+"/"+QString::number(BuildID)+".7z");
    QString extractPath(extractDirectory+"/"+QString::number(BuildID));
    QString buildPath(allBuildsDirectory + "/" + QString::number(BuildID));

    QDir eDir(extractPath);
    if(!eDir.exists()){
        QDir().mkpath(extractPath);
    }

    Compression c;
    c.decompress(zipPath, extractPath);

    QFile zipDelete(zipPath);
    if(zipDelete.exists())
        zipDelete.remove();

    QStringList listOfPaths;
    DirectoryHandler::recurseAddDir(eDir, listOfPaths);

    DirectoryHandler::copyOverFromList(3, listOfPaths, buildPath, extractPath);

    DirectoryHandler::removeDir(extractPath);
}
