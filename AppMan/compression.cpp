#include "compression.h"
#include <QString>
#include <QProcess>

compression::compression(Build* b)
{
    build = b;
}

void compression::compress(){
    //Currently compresses <BuildDirectory> and stores <BuildName>.7z file in local directory.
    //Requires 7z.exe in local directory
    //Only works in Windows (so far)

    QString s("7z");
    QStringList args;
    args.append("a");
    args.append(build->getBuildName() + ".7z");
    args.append(build->getBuildDirectory());
    QProcess* p = new QProcess();
    p->start(s,args);
}

void compression::decompress(){
    //Currently decompresses local <BuildName>.7z file and extracts in local directory.
    //Requires 7z.exe in local directory
    //Only works in Windows (so far)

    QString s("7z");
    QStringList args;
    args.append("x");
    args.append(build->getBuildName() + ".7z");
    args.append("-y");
    QProcess* p = new QProcess();
    p->start(s,args);
}
