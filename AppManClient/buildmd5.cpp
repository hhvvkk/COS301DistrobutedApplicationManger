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
    currentIndex = 0;
    md5perthread = new QByteArray[threadCount];
}

BuildMD5::~BuildMD5()
{
    md5perthread->clear();
    delete md5perthread;
    buildFiles->clear();
    delete buildFiles;
    buildFilesMD5->clear();
    delete buildFilesMD5;
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

const QString* BuildMD5::getCurrentBuildDirectory() const{
    if(buildFiles->isEmpty())
        return NULL;
    else if(currentIndex >= buildFiles->size())
        return NULL;

    return &buildFiles->at(currentIndex);
}

const QString* BuildMD5::getCurrentFileMd5Sum() const{
    if(buildFilesMD5->isEmpty())
        return NULL;
    else if(currentIndex >= buildFiles->size())
        return NULL;

    return &buildFilesMD5->at(currentIndex);
}

void BuildMD5::next(){
    //a Simple iterator to loop through the builds
    currentIndex++;
}

int BuildMD5::getCurrentIndex(){
    return currentIndex;
}

int BuildMD5::getSize(){
    return buildFiles->size();
}
