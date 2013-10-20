#include "myDirIterator.h"

myDirIterator::myDirIterator(QString path){
    QDir direc(path);
    it = new QDirIterator(direc, QDirIterator::NoIteratorFlags);
}

myDirIterator::myDirIterator(QString path, int flag){
    QDir direc(path);
    if (flag == 1)
        it = new QDirIterator(direc, QDirIterator::Subdirectories);
    else
        it = new QDirIterator(direc, QDirIterator::NoIteratorFlags);
}

myDirIterator::~myDirIterator(){
    delete it;
}

void myDirIterator::getFileInfo(){
    int cnt = 0;
    while(it->hasNext()){
        it->next();
        QFileInfo fileInfo = it->fileInfo();
        QDateTime date = fileInfo.lastModified();
        QString strDate = date.toString();

        QDir dirTest(it->fileName());

        if(dirTest.exists()){
            //it is a directory, ignore therefore
        }
        else //it is a file or other, thus include
        if(it->fileName().compare("..") && it->fileName().compare(".")) {
            //if the file names are not . or .. continue
            fileMods.push_back(strDate);
            fileNames.push_back(it->fileName());
            filePaths.push_back(it->filePath());
            cnt++;
        }
    }
}
