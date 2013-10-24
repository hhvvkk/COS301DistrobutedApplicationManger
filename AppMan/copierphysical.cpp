#include "copierphysical.h"

CopierPhysical::CopierPhysical(int machID, int theBuildID, QString pathForZip, QObject *parent) :
    QTcpServer(parent),
    machineID(machID),
    BuildID(theBuildID),
    zipPath(pathForZip)//,
    //management(man)
{
    i = 0;
    maxPerSize = 1024;

    //gaan possibly user maak die self set...
    notifyTimer.setInterval(3000);
    connect(&notifyTimer, SIGNAL(timeout()), this, SLOT(signalNotifyProgress()));
}

CopierPhysical::~CopierPhysical(){
    notifyTimer.stop();

    this->close();
    socket->disconnectFromHost();
    socket->deleteLater();
}

int CopierPhysical::startServer(){
    if(!this->listen(QHostAddress::Any)){
        //qDebug() << "Could not start server";
    }
//    else
//        qDebug() << "SenderServerListening...";

    //returns a zero if it is not listening otherwise a server port
    return this->serverPort();
}


void CopierPhysical::stopServer(){
    this->close();
}


int CopierPhysical::getBuildID(){
    return this->BuildID;
}

void CopierPhysical::incomingConnection(int socketID){
    socket = new QTcpSocket();

    //set the socket descriptor to that client which connected
    socket->setSocketDescriptor(socketID);


    if(!socket){
        qDebug()<<"Unable to send files";
        destroyServer();
        return;
    }

    connect(socket, SIGNAL(disconnected()), this, SLOT(destroyServer()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyReadFunction()));
    stopServer();

}


void CopierPhysical::destroyServer(){
    this->deleteLater();
}



void CopierPhysical::initiateCopyOver(){
    QFile zipFile(zipPath);
    if(!zipFile.exists()){
        return;
    }

    if(!zipFile.open(QFile::ReadOnly)){
        //if it fails to open the file
        emit copierPhysicalDone(this->BuildID);
        return;
    }

    //concurrently read all from buffer...

    QFuture <QByteArray>futureReadFile = QtConcurrent::run(this, &CopierPhysical::concurrentFileRead, &zipFile);
    buffer = futureReadFile.result();

    writeFileOverNetwork();

    //QFuture <void>endWrittenFuture = QtConcurrent::run(this, &CopierPhysical::writeFileOverNetwork);
    //endWrittenFuture.waitForResult();


    //emit that it is done so that the copysenderClient can be notified...
    emit copierPhysicalDone(this->BuildID);
    signalNotifyProgress();
    notifyTimer.stop();

}

void CopierPhysical::writeFileOverNetwork(){
    socket->moveToThread(QThread::currentThread());
    notifyTimer.start();
    for(; i < buffer.size();){
        //QByteArray  mid ( int pos, int len = -1 ) const
        QByteArray midToWrite = buffer.mid(i, maxPerSize);
        qint64 written = socket->write(midToWrite);
        if(socket->isWritable()){
            socket->waitForBytesWritten(-1);
        }
        else{
            break;
        }
        //change the speed at which it writes...
        i = i + written;
    }
}

QByteArray CopierPhysical::concurrentFileRead(QFile *fileToRead){
     return fileToRead->readAll();
}

void CopierPhysical::signalNotifyProgress(){
    emit notifyProgress(i, buffer.size(), BuildID);
    notifyTimer.start();
}

void CopierPhysical::readyReadFunction(){
    QString data = socket->readAll();
    if(!data.compare("HelloPhysicalSender"))
        initiateCopyOver();
    else
        emit copierPhysicalDone(this->BuildID);
}
