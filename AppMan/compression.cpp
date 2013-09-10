#include "compression.h"
#include <QString>
#include <QProcess>

compression::compression() {
}

void compression::compress(QStringList dirs, QString toDir, QString name){
    #ifdef WIN32
        QString s("7zip/Win32/7z");
        QStringList args;
        args.append("a");
        args.append(toDir + name + ".7z"); //**
        args.append(dirs);
        QProcess* p = new QProcess();
        p->start(s,args);
    #else
        QString s("7zip/Unix/7z");
        QStringList args;
        args.append("a");
        args.append("-ttar");
        args.append(toDir + name + ".tar"); //**
        args.append(dirs);
        QProcess* p = new QProcess();
        p->start(s,args);
    #endif
}

void compression::decompress(QString fromDir, QString toDir, QString name){
    #ifdef WIN32
        QString s("7zip/Win32/7z");
        QStringList args;
        args.append("x");
        args.append(fromDir + name + ".7z"); //**
        args.append("-y");
        args.append("-o" + toDir);
        QProcess* p = new QProcess();
        p->start(s,args);
    #else
        QString s("7zip/Unix/7z");
        QStringList args;
        args.append("x");
        args.append(fromDir + name + ".tar"); //**
        args.append("-y");
        args.append("-o" + toDir);
        QProcess* p = new QProcess();
        p->start(s,args);
    #endif
}


// ** make sure <fromDir> contains a path that ends with </> else this line has to be modified to:  args.append(fromDir + "/" + name + ".7z")

/* SAMPLE USAGE:
 *
 *  compression* c = new compression();
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
