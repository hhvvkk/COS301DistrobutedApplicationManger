#include "copycompare.h"

CopyCompare::CopyCompare(QObject *parent) :
    QObject(parent)
{
    filePaths = new QStringList();
    deleteFilePaths = new QStringList();
    numFilesNotCopied = 0;
}

CopyCompare::~CopyCompare(){
    delete filePaths;
    delete deleteFilePaths;
}


QStringList CopyCompare::getFilepaths(){
    return *filePaths;
}

void CopyCompare::compareMD5(const QString *masterMD5Value, const QString *masterFilePath, QString slaveMd5Value){
    //Will now compare whether or not the file should be included in the build
    if(masterMD5Value->compare(slaveMd5Value)){//if it is the same, it should not be included in copy over
        filePaths->append(*masterFilePath);
    }
    else{
        numFilesNotCopied++;
    }
}

void CopyCompare::createFile(QString aPath){
    filePaths->append(aPath);
}

double CopyCompare::percentageSynched(){
    double percentageComplete = 0;
//   qDebug()<<"numNotCopied:"<<numFilesNotCopied;
//   qDebug()<<"filesToCopy:"<<filePaths->size();
//   qDebug()<<"filesToDelete:"<<deleteFilePaths->size();

    int total = numFilesNotCopied + filePaths->size();

    percentageComplete = 100.00 - filePaths->size()*100/total;
    return percentageComplete;
}

void CopyCompare::deleteFile(const QString file){
    deleteFilePaths->append(file);
}

QString CopyCompare::getDeleteJsonString(QString buildNo){
    if(deleteFilePaths->isEmpty())
        return "";

    QString jsonDeleteString = "{";
    jsonDeleteString.append("\"handler\" : \"DeleteFilesList\" , \"buildNo\" : \""+buildNo+"\", \"filePaths\": [ ");

    for(int i = 0; i <deleteFilePaths->size(); i++){
        if(i < (deleteFilePaths->size() -1))
            jsonDeleteString.append("{\"d\" : \""+deleteFilePaths->at(i)+"\"},");
        else
            jsonDeleteString.append("{\"d\" : \""+deleteFilePaths->at(i)+"\"}");
    }

    jsonDeleteString.append(" ] }");
    return jsonDeleteString;
}
