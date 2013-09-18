#include "copierphysical.h"

CopierPhysical::CopierPhysical(int machID, int buildNumber, QString pathForZip, QObject *parent) :
    QTcpServer(parent),
    machineID(machID),
    buildNo(buildNumber),
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

//void CopierPhysical::readyReadFunction(){

//    QByteArray readSizes = socket->readAll();
//    QStringList sizesSent;

//    while(readSizes.length() != 1){
//        sizesSent << readSizes.mid(1,(readSizes.indexOf("|")-1));
//        readSizes.remove(0, readSizes.indexOf("|"));
//    }

//    QFile zipFile(zipPath);
//    if(!zipFile.exists()){
//        qDebug()<<"DOES not exist::"<<zipPath;
//        return;
//    }
//    if(!zipFile.open(QFile::ReadOnly)){
//        qDebug()<<"unable to open for copy";
//        return;
//    }

//    for(int counter = 0; counter < sizesSent.length(); counter++){
//        i += sizesSent.at(i).toInt();
//    }

//    //QByteArray buffer = zipFile.readAll();
//    int maxPerSize = 1024;
//    for(; i < buffer.size();){
//        //QByteArray  mid ( int pos, int len = -1 ) const
//        QByteArray midToWrite = buffer.mid(i,maxPerSize);
//        socket->write(midToWrite);
//        socket->flush();
//    }
    //zipFile.close();
//}


void CopierPhysical::initiateCopyOver(){

    QFile zipFile(zipPath);
    if(!zipFile.exists()){
        return;
    }
    if(!zipFile.open(QFile::ReadOnly)){
        qDebug()<<"unable to open for copy";
        return;
    }

    buffer = zipFile.readAll();
    notifyTimer.start();
    for(; i < buffer.size();){
        //QByteArray  mid ( int pos, int len = -1 ) const
        QByteArray midToWrite = buffer.mid(i, maxPerSize);
        qint64 written = socket->write(midToWrite);
        socket->flush();
        //change the speed at which it writes...
        i = i + written;
    }

    //emit that it is done so that the copysenderClient can be notified...
    emit copierPhysicalDone(this->buildNo);
    signalNotifyProgress();
    notifyTimer.stop();

    //dont delete yet
    // socket->disconnectFromHost();
    // socket->deleteLater();
    // socket = 0;


    /*
     QByteArray ba("Hello world");
     char *data = ba.data();
     while (*data) {
         cout << "[" << *data << "]" << endl;
         ++data;
     }*/


//    while(!zipFile.atEnd()){
//        //qint64 readThis = zipFile.read(buffer, maxBufferSize);
//        //QByteArray QIODevice::read ( qint64 maxSize )
//        QByteArray buffer = zipFile.read(maxBufferSize);
//        if(buffer.size() != 0){
//            if(socket->isWritable()){
//                /*Writes at most maxSize bytes of data
//                    from data to the device. Returns the
//                     number of bytes that were actually written,
//                    or -1 if an error occurred.
//                    */
//                qint64 written = socket->write(buffer);
//                socket->flush();
//                maxBufferSize = written;
//            }else{
//                qDebug()<<"IT IS not writable!!";
//                break;
//            }
//        }else{
//            break;
//        }
//    }












    //    buffer.clear();
    //    //start writing...
    //    QFile zipFile(zipPath);
    //    if(!zipFile.exists()){
    //        qDebug()<<"DOES not exist::"<<zipPath;
    //        return;
    //    }
    //    if(!zipFile.open(QFile::ReadOnly)){
    //        qDebug()<<"unable to open for copy";
    //        return;
    //    }

    //    buffer = zipFile.readAll();
    //    int maxPerSize = 1024;
    //    for(; i < buffer.size();){
    //        //QByteArray  mid ( int pos, int len = -1 ) const
    //        QByteArray midToWrite = buffer.mid(i,maxPerSize);
    //        socket->write(midToWrite);
    //        socket->flush();
    //    }
    //    zipFile.close();











//    QFile zipFile(zipPath);
//    if(!zipFile.exists()){
//        qDebug()<<"DOES not exist::"<<zipPath;
//        return;
//    }
//    if(!zipFile.open(QFile::ReadOnly)){
//        qDebug()<<"unable to open for copy";
//        return;
//    }

//    qint64 maxBufferSize = 100;


//    while(!zipFile.atEnd()){
//        //qint64 readThis = zipFile.read(buffer, maxBufferSize);
//        //QByteArray QIODevice::read ( qint64 maxSize )
//        QByteArray buffer = zipFile.read(maxBufferSize);
//        if(buffer.size() != 0){
//            if(socket->isWritable()){
//                /*Writes at most maxSize bytes of data
//                    from data to the device. Returns the
//                     number of bytes that were actually written,
//                    or -1 if an error occurred.
//                    */
//                qint64 written = socket->write(buffer);
//                socket->flush();
//                maxBufferSize = written;
//            }else{
//                qDebug()<<"IT IS not writable!!";
//                break;
//            }
//        }else{
//            break;
//        }
//    }


    //    QFile zipFile(zipPath);
    //    if (!zipFile.open(QIODevice::ReadOnly))
    //        return;

    //    QTextStream os(socket);
    //    //os.setAutoDetectUnicode(true);

    //    // Streaming the file
    //    QByteArray block = zipFile.readAll();
    //    socket->write(block);
    //    socket->flush();



    /*while (1) {
        // Read data into buffer.  We may not have enough to fill up buffer, so we
        // store how many bytes were actually read in bytes_read.
        int bytes_read = read(input_file, buffer, sizeof(buffer));
        if (bytes_read == 0) // We're done reading from the file
            break;

        if (bytes_read < 0) {
            // handle errors
        }

        // You need a loop for the write, because not all of the data may be written
        // in one call; write will return how many bytes were written. p keeps
        // track of where in the buffer we are, while we decrement bytes_read
        // to keep track of how many bytes are left to write.
        void *p = buffer;
        while (bytes_read > 0) {
            int bytes_written = write(output_socket, p, bytes_read);
            if (bytes_written <= 0) {
                // handle errors
            }
            bytes_read -= bytes_written;
            p += bytes_written;
        }
    }*/
}

void CopierPhysical::signalNotifyProgress(){
    emit notifyProgress(i, buffer.size(), buildNo);
    notifyTimer.start();
}

void CopierPhysical::readyReadFunction(){
    QString data = socket->readAll();
    if(!data.compare("HelloPhysicalSender"))
        initiateCopyOver();
    qDebug()<<"STarting the copy...";
}
