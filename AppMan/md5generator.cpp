#include "md5generator.h"

//Sample usage

//#include "md5generator.h"
//#include <QElapsedTimer>

//qDebug() << "Loading...";
//QElapsedTimer timer;
//timer.start();
//md5generator md5("C:/md5/",5); <--- 5 is number of threads
//md5.generate();
//QStringList* dirs = md5.getDirs();
//QStringList* md5s = md5.getMd5s();
//for (int i=0; i< dirs->size();i++){
//    qDebug() << dirs->at(i) + " " + md5s->at(i);
//}
//qDebug() << "Directory MD5: " + md5.getDirectoryMD5();
//qDebug() << "MD5 took " << timer.elapsed() << "milliseconds to complete";

md5generator::md5generator(QString directory, int threadCnt)
{
    dir = directory;
    buildFiles = new QStringList();
    buildFilesMD5 = new QStringList();    
    finished = 0;
    threadCount = threadCnt;
    dirHash = new QCryptographicHash(QCryptographicHash::Md5);
    lock = new QMutex();
}

md5generator::~md5generator()
{
    buildFiles->clear();
    delete buildFiles;
    buildFilesMD5->clear();
    delete buildFilesMD5;    
}

void md5generator::generate() {
    finished = 0;
    dirHash->reset();

    myDirIterator dirIt(dir,1);
    dirIt.getFileInfo();
    QVector<QString> paths = dirIt.retrieveFilePaths();

    int dirCnt = paths.size();
    if (threadCount > dirCnt)
            threadCount = dirCnt;
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

        threads.push_back(new md5Thread(fileSplit,this));
        connect(threads.at(i), SIGNAL(finished()), threads.at(i), SLOT(deleteLater()));
        threads.at(i)->start();
   }

    while (finished != threadCount){
    }
}

void md5generator::patchThreads(QStringList* dirs, QStringList* dirsMD5, QByteArray mainHash){
    lock->lock();    
    dirHash->addData(mainHash.constData());
    buildFiles->append(dirs->mid(0));
    buildFilesMD5->append(dirsMD5->mid(0));
    directoryMD5 = dirHash->result().toHex().constData();
    finished++;
    lock->unlock();
}
