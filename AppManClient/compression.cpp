#include "compression.h"
#include <QString>
#include <QProcess>

#include <QDir>

#include <QFile>

#include <QDebug>

Compression::Compression(QObject *parent)
    :QObject(parent){

    //which means 15 files maximum will be copied per try
    partitionSize = 15;

    //What this constructor does is create all the folders for 7zip
    //if it does not exist and copies the 7zip from the resource file
    //into that folder
    QDir compressor("7zip");

    if(!compressor.exists()){
       QDir().mkdir("7zip");
    }

    #ifdef WIN32
        QDir win("7zip/Win32");
        if(!win.exists()){
            QDir().mkdir("7zip/Win32");
        }
        QFile svenZ("7zip/Win32/7z.exe");
        if(!svenZ.exists()){
            QFile::copy(":/SevenZip/7zip/Win32/7z.exe", "7zip/Win32/7z.exe");
            QFile theCopiedFile("7zip/Win32/7z.exe");
            theCopiedFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther);
        }
        QFile dll("7zip/Win32/7z.dll");
        if(!dll.exists()){
            QFile::copy(":/SevenZip/7zip/Win32/7z.dll", "7zip/Win32/7z.dll");
            QFile theCopiedFile("7zip/Win32/7z.dll");
            theCopiedFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther);
        }
    #else
        QDir linx("7zip/Unix");
        if(!linx.exists()){
            QDir().mkdir("7zip/Unix");
        }
        QFile svenZ("7zip/Unix/7za");
        if(!svenZ.exists()){
            QFile::copy(":/SevenZip/7zip/Unix/7za", "7zip/Unix/7za");
            QFile theCopiedFile("7zip/Unix/7za");
            theCopiedFile.setPermissions(QFile::ReadOwner | QFile::WriteOwner | QFile::ExeOwner | QFile::ReadGroup | QFile::ExeGroup | QFile::ReadOther | QFile::ExeOther);
        }
    #endif

}

void Compression::compress(QStringList dirs, QString toDir, QString buildDirectory){

    //copy all files into the directories(in order to keep the directory structures
    //7zip does not have commands to force the directories to be kept
    int buildDirectorySize = buildDirectory.size();
    for(int i = 0; i < dirs.length(); i++){
        QString directoryToCopy = dirs.at(i);
        directoryToCopy.remove(0, buildDirectorySize);
        QString whereToCopy = toDir + directoryToCopy;
        createFolderPaths(whereToCopy);
        //delete file first if there already exist such a one...
        bool copied = QFile().copy(dirs.at(i), whereToCopy);
        if(!copied){
            //qDebug()<<"did not copy "<<dirs.at(i)<<" to "<<whereToCopy;
        }
    }

    #ifdef WIN32
        QString s("7zip/Win32/7z.exe");
    #else
        QString s("7zip/Unix/7za");
    #endif

    QString compressDirectory = toDir  + ".7z";

    //Finally compress the files in folders into a 7zip file
    QStringList args;
    args.append("a"); // archive
    //args.append("-mx=5"); // archive
    args.append("-bd"); //disable percentage sign
    //args.append("-r"); //recursively add files //maby not needed...works without it
    args.append("-mmt"); //multithreaded
    args.append(compressDirectory);//where will be compressed
    QString whereBuildFilesAre = toDir;
    whereBuildFilesAre += "/";
    args.append(whereBuildFilesAre);//the directory where the build files are contained to be compressed...

    QProcess* p = new QProcess();
    //connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(endProcess(int, QProcess::ExitStatus)));
    p->start(s,args);
    p->waitForFinished(40000);
    p->kill();
    p->deleteLater();


//    int counter = 0;
//    int finalCounter = 0;
//    while(counter < dirs.size()){

//        QStringList args;
//        args.append("a");
//        args.append("-bd");
//        args.append("-mmt");
//        args.append(compressDirectory);
//        args.append("dir\blah");

//        int i = counter;
//        for(; i < (counter + partitionSize) && (i < dirs.size()); i++){
//            args.append(dirs.at(i));
//        }
//        finalCounter = i;
//        qDebug()<<args;

//        QProcess* p = new QProcess();
//        //connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(endProcess(int, QProcess::ExitStatus)));

//        p->start(s,args);

//        p->waitForFinished(4000);
//        p->kill();
//        p->deleteLater();
//        counter += partitionSize;
//        finalCounter += partitionSize;
//    }

//    int counterForLast = finalCounter - partitionSize;
//    QStringList args;
//    args.append("a");
//    args.append("-bd");
//    args.append("-mmt");
//    args.append(compressDirectory);
//    args.append("dir\blah");
//    qDebug()<<"size= "<< dirs.size() <<"-- -Counter was "<<counter<<"STILL TO DO:" <<counterForLast << "--to--"<<dirs.size();
//    for(int i = counterForLast; i < dirs.size(); i++){
//        args.append(dirs.at(i));
//    }
//    QProcess* p = new QProcess();
//    //connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(endProcess(int, QProcess::ExitStatus)));
//    p->start(s,args);

//    p->waitForFinished(4000);
//    p->kill();
//    p->deleteLater();
    //finally compress the last few files..
}

void Compression::createFolderPaths(QString folderPath){
    bool firstDone = false;
    QString currentFolder = "";
    while(folderPath.contains("/")){
        if(folderPath.contains("/")){
            QString folder = folderPath.left(folderPath.indexOf("/"));
            QDir existFolder(folder);
            if(!existFolder.exists()){
                if(!firstDone)
                    QDir().mkdir(folder);
                else
                    QDir().mkdir(currentFolder+"/"+folder);
            }
            if(!firstDone){
                currentFolder.append(folder);
                firstDone = true;
            }
            else
                currentFolder.append("/"+folder);
            folderPath.remove(0, (folderPath.indexOf("/")+1));
        }
    }
}

void Compression::decompress(QString zipPath, QString toDir){
    #ifdef WIN32
        QString s("7zip/Win32/7z.exe");
    #else
        QString s("7zip/Unix/7za");
    #endif

    QStringList args;
    args.append("x");
    args.append("-y");
    args.append("-o" + toDir);
    args.append(zipPath);

    QProcess* p = new QProcess();
    p->start(s,args);
    p->waitForFinished(40000);
    p->kill();
}


bool Compression::zipInTact(QString directoryOfZip){
    QFile zipFile(directoryOfZip);

    //firstly check if file is there
    if(!zipFile.exists())
        return false;

    //use the test function of 7zip [t]
    QString s("7zip/Unix/7za");
    QStringList args;
    args.append("t");
    args.append(directoryOfZip);
    QProcess* p = new QProcess();

    p->start(s, args);
    p->waitForFinished(-1);

    //QString p_stdout = p->readAllStandardOutput();
    QByteArray allOutput = p->readAll();

    if(allOutput.contains("Everything is Ok"))
        return true;
    else
        return false;
}

// ** make sure <fromDir> contains a path that ends with </> else this line has to be modified to:  args.append(fromDir + "/" + name + ".7z")

/* SAMPLE USAGE:
 *
 *  Compression* c = new Compression();
 *
 *  QStringList dirList;
 *
 *  dirList.append("somepath/somefile1.txt");
 *  dirList.append("somepath2/somefile2.txt");
 *  dirList.append("somepath3/somedirectory");
 *
 *  c->compress(dirList,"path/ToDir/","compressedFileName");
 *
 *  c->decompress("path/fromDir/","path/new/ToDir/","decompressedDirName");
 *
 **/
