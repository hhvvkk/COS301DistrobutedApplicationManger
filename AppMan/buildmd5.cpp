#include "buildmd5.h"

BuildMD5::BuildMD5(QObject *parent) :
    QObject(parent)
{
    currentIndex = 0;
}

BuildMD5::~BuildMD5(){
    buildFiles->clear();
    delete buildFiles;
    buildFilesMD5->clear();
    delete buildFilesMD5;
}

void BuildMD5::generateAllMD5(QString dir){
    //will insert the md5 for each file inside the QStringlist buildFiles and QStringList filesMd5s

    myDirIterator dirIt(dir,1);
    dirIt.getFileInfo();
    QVector<QString> paths = dirIt.retrieveFilePaths();

    buildFiles = new QStringList();    // ** may create memory leaks
    buildFilesMD5 = new QStringList();

    for (int i=0;i<paths.size();i++) {
        QFile file(paths.at(i));
        QDir directory(paths.at(i));

        if(!directory.exists()){//if it is not a directory, will add it

            //omit directories from md5 results
            file.open(QFile::ReadOnly);

            QCryptographicHash md5(QCryptographicHash::Md5);
            md5.addData(file.readAll());
            buildFiles->append(paths.at(i));
            QString hash(md5.result().toHex().constData());
            buildFilesMD5->append(hash);

            file.close();
            md5.reset();
        }
    }
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
