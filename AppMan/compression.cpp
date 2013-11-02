#include "compression.h"

Compression::Compression(QObject *parent)
    :QObject(parent){

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

    int maximumBytes = 1000000000;//maximum for byteArray is //2147483647, therefore only allow this amount

    int currentBytes = 0;

    for(int i = 0; i < dirs.length(); i++){
        QString directoryToCopy = dirs.at(i);
        directoryToCopy.remove(0, buildDirectorySize);
        QString whereToCopy = toDir + directoryToCopy;

        QDir().mkpath(whereToCopy.left(whereToCopy.lastIndexOf("/")));

        //delete file first if there already exist such a one...
        QFile copyFile(dirs.at(i));

        currentBytes += copyFile.size();

        bool copied = copyFile.copy(whereToCopy);
        copyFile.waitForBytesWritten(-1);
        //if(!copied){

       // }

        //limit the amount to send if it goes over it...
        if(currentBytes >= maximumBytes){
            //
            qDebug()<<"the following file makes it over"<<dirs.at(i);
            qDebug()<<"--SIZ:"<<copyFile.size();
            break;
        }
    }

    #ifdef WIN32
        QString s("7zip/Win32/7z.exe");
    #else
        QString s("7zip/Unix/7za");
    #endif

    QString compressDirectory = toDir  + ".7z";

    QDir theCompressDirTest(compressDirectory);
    QFile theCompressFileTest(compressDirectory);

    if(theCompressDirTest.exists()){
        theCompressDirTest.remove(compressDirectory);
    }
    if(theCompressFileTest.exists()){
        theCompressFileTest.remove(compressDirectory);
        theCompressFileTest.waitForBytesWritten(15);
    }

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
    p->waitForFinished(-1);
    p->kill();
    p->deleteLater();

    if(!zipInTact(compressDirectory)){
        qDebug()<<"ZIP NOT IN TACT";
        compress(dirs, toDir, buildDirectory);
    }
    deleteAllFiles(toDir);

}

void Compression::deleteAllFiles(QString compressDirectory){
    DirectoryHandler::removeDir(compressDirectory);
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
    p->waitForFinished(-1);
    p->kill();
    p->deleteLater();
}


bool Compression::zipInTact(QString directoryOfZip){
    QFile zipFile(directoryOfZip);

    //firstly check if file is there
    if(!zipFile.exists())
        return false;

    //use the test function of 7zip [t]
    #ifdef WIN32
        QString s("7zip/Win32/7z.exe");
    #else
        QString s("7zip/Unix/7za");
    #endif

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

