#include "buildmd5.h"

BuildMD5::BuildMD5(QObject *parent) :
    QObject(parent)
{
}

void BuildMD5::generateAllMD5(Build build){
    //will insert the md5 for each file inside the QStringlist buildFiles and QStringList filesMd5s    

    QString dir = build->getBuildDirectory();

    myDirIterator dirIt(dir,1);
    dirIt.getFileInfo();
    QVector<QString> paths = dirIt.retrieveFilePaths();

    buildFiles = new QStringList[paths.size()];    // ** may create memory leaks
    buildFilesMD5 = new QStringList[paths.size()]; // **

    for (int i=0;i<paths.size();i++) {
        QFile file(paths.at(i));
        file.open(QFile::ReadOnly);

        QCryptographicHash md5(QCryptographicHash::Md5);
        md5.addData(file.readAll());

        buildFiles[i] = paths.at(i);
        //qDebug()<< "buildFiles[ " + i + "]: " + paths.at(i);

        QString hash(md5.result().toHex().constData());
        buildFilesMD5[i] = hash;
        //qDebug()<< "buildFilesMD5[ " + i + "]: " + hash;

        file.close();
        md5.reset();
    }
}
