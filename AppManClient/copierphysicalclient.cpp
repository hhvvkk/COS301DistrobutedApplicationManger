#include "copierphysicalclient.h"

CopierPhysicalClient::CopierPhysicalClient(QHostAddress hAdr, int portNumber, int bNumber,  QObject *parent) :
    QObject(parent),
    port(portNumber),
    hostAddress(hAdr),
    buildNo(bNumber)
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
        QDir().mkdir(allBuildsDirectory);

    QDir allCDir(compressDirectory);
    if(!allCDir.exists())
        QDir().mkdir(compressDirectory);

    QDir allEDir(extractDirectory);
    if(!allEDir.exists())
        QDir().mkdir(extractDirectory);

    amountRead = 0;
}

CopierPhysicalClient::~CopierPhysicalClient(){
    qDebug()<<"deleting copierphysical client";
}


int CopierPhysicalClient::getBuildNo(){
    return buildNo;
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
    qDebug()<<"Disconnected called";
   // this->deleteLater();
}

void CopierPhysicalClient::readyReadFunction(){
    //read all from the socket at that stage
    QByteArray Data = socket->readAll();

    //write to the file buffer the data that was read
    fileBuffer.append(Data);
}

void CopierPhysicalClient::writeToFile(){
    //write the full buffer to the zip file...

    QFile zipFile(compressDirectory+"/"+QString::number(buildNo)+".7z");

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

	//zipCopySuccess
    emit doneWritingToFile(buildNo, true);

    if(zipCopySuccess){
        extractZipToDirectory();
    }

    //disconnect from the other machine
    socket->disconnectFromHost();
}


void CopierPhysicalClient::removeZipFile(){
    QFile zipFile(compressDirectory+"/"+QString::number(buildNo)+".7z");

    if(zipFile.exists()){
        zipFile.remove();
    }

}

bool CopierPhysicalClient::zipInTact(){
    Compression c;

    QString zipPath(compressDirectory+"/"+QString::number(buildNo)+".7z");
    //determine whether the zip is in tact. Return true if it is, else false;
    bool zipInTact = c.zipInTact(zipPath);

    //if it was in tact, extract it in the right directory after returning
    return zipInTact;
}


void CopierPhysicalClient::extractZipToDirectory(){
    QString zipPath(compressDirectory+"/"+QString::number(buildNo)+".7z");
    QString extractPath(extractDirectory+"/"+QString::number(buildNo));

    QDir eDir(extractPath);
    if(!eDir.exists()){
        QDir().mkpath(extractPath);
    }

    Compression c;
    c.decompress(zipPath, extractPath);

    QFile zipDelete(zipPath);
    if(zipDelete.exists())
        zipDelete.remove();


}
