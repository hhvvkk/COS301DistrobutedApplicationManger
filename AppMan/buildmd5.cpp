#include "buildmd5.h"

//Sample usage

//#include "BuildMD5.h"
//#include <QElapsedTimer>

//qDebug() << "Loading...";
//QElapsedTimer timer;
//timer.start();
//BuildMD5 md5("C:/md5/",5); <--- 5 is number of threads
//md5.generate();
//QStringList* dirs = md5.getDirs();
//QStringList* md5s = md5.getMd5s();
//for (int i=0; i< dirs->size();i++){
//    qDebug() << dirs->at(i) + " " + md5s->at(i);
//}
//qDebug() << "Directory MD5: " + md5.getDirectoryMD5();
//qDebug() << "MD5 took " << timer.elapsed() << "milliseconds to complete";

BuildMD5::BuildMD5(QString directory, int threadCnt)
{
    dir = directory;
    buildFiles = new QStringList();
    buildFilesMD5 = new QStringList();
    finished = 0;
    threadCount = threadCnt;
    dirHash = new QCryptographicHash(QCryptographicHash::Md5);
    lock = new QMutex();
    md5perthread = 0;//will be initiated later
    this->old = false;
    this->inUse = true;
    users = 0;
}

BuildMD5::~BuildMD5(){
    //threads delete themself
    //connect(finished...)...
    if(md5perthread)
        delete [] md5perthread;//delete an array
    buildFiles->clear();
    delete buildFiles;
    buildFilesMD5->clear();
    if(buildFilesMD5)
        delete buildFilesMD5;
    if(dirHash)
        delete dirHash;
}

void BuildMD5::generate() {
    finished = 0;
    dirHash->reset();
    myDirIterator dirIt(dir,1);
    dirIt.getFileInfo();
    QVector<QString> paths = dirIt.retrieveFilePaths();

    int dirCnt = paths.size();
    if (threadCount > dirCnt)
            threadCount = dirCnt;
    if (threadCount == 0){
        directoryMD5 = "0";
        return;
    }
    md5perthread = new QByteArray[threadCount];
    int dirsPerThread = dirCnt / threadCount;

    for (int i=0;i<threadCount;i++){

        QStringList* fileSplit = new QStringList;
        for (int j=i*dirsPerThread;j < dirsPerThread*(i+1);++j){
            fileSplit->append(paths.at(j));
        }
        if (i == threadCount-1) {
            for (int j=dirsPerThread*threadCount;j < dirCnt;++j){
                fileSplit->append(paths.at(j));
            }
        }

        threads.push_back(new md5Thread(i, fileSplit,this));
        connect(threads.at(i), SIGNAL(finished()), threads.at(i), SLOT(deleteLater()));
        threads.at(i)->start();
   }
    while (finished != threadCount){
    }
    for (int i=0;i<threadCount;i++){
        dirHash->addData(md5perthread[i]);
    }

    directoryMD5 = dirHash->result().toHex().constData();
}

void BuildMD5::patchThreads(int threadNumber, QStringList* dirs, QStringList* dirsMD5, QByteArray mainHash){
    lock->lock();
    //dirHash->addData(mainHash.constData());
    md5perthread[threadNumber] = mainHash;
    buildFiles->append(dirs->mid(0));
    buildFilesMD5->append(dirsMD5->mid(0));
    //directoryMD5 = dirHash->result().toHex().constData();
    finished++;
    lock->unlock();
}

const QString* BuildMD5::getCurrentBuildDirectory(int i) const{
    if(buildFiles->isEmpty())
        return NULL;
    else if(i >= buildFiles->size())
        return NULL;

    return &buildFiles->at(i);
}

const QString* BuildMD5::getCurrentFileMd5Sum(int i) const{
    if(buildFilesMD5->isEmpty())
        return NULL;
    else if(i >= buildFiles->size())
        return NULL;

    return &buildFilesMD5->at(i);
}

int BuildMD5::getSize(){
    return buildFiles->size();
}

void BuildMD5::setIsOld(bool value){
    if(value)
        qDebug()<<"yes";
    if(!this->old)
        qDebug()<<"No";
    this->old = value;
}

bool BuildMD5::isOld(){
    return this->old;
}

void BuildMD5::setIsInUse(bool value){
    deleteLock.lock();
    if(value == true)
        users++;
    else
        users--;

    this->inUse = value;
    deleteLock.unlock();
}


void BuildMD5::tryDelete(){
    lock->lock();
    //looks to see whether there is nobody more using this class,
    //if there are none delete it
    if(users == 0)
        this->deleteLater();
    lock->unlock();
}
