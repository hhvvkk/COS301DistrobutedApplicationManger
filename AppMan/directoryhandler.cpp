#include "directoryhandler.h"
#include <QDebug>

DirectoryHandler::DirectoryHandler(QObject *parent) :
    QObject(parent)
{
}

void DirectoryHandler::recurseAddDir(QDir d, QStringList & list) {

    QStringList skipList = d.entryList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);

    foreach (QString file, skipList) {

        QFileInfo finfo(QString("%1/%2").arg(d.path()).arg(file));

        if (finfo.isSymLink())
            return;

        if (finfo.isDir()) {
            QDir anotherDir(finfo.filePath());
            recurseAddDir(anotherDir, list);
        } else
            list << QDir::toNativeSeparators(finfo.filePath());
    }
}

void DirectoryHandler::copyOverFromList(int depth, QStringList &list, QString whereTo, QString pathOfRoot){
    if(whereTo.at(whereTo.size()-1) != '/'){
        whereTo.append("/");
    }

    for(int i = 0; i < list.size(); i++){
        //:::  /root/Desktop/ProjectFolder/AppManClient-build-desktop/extract/1/buildCompressed/42/dir/file.txt
        //:::  /root/Desktop/ProjectFolder/AppManClient-build-desktop/extract/1/buildCompressed/42/file2.txt
        QString fullPath = list.at(i);
        QString fileName = fullPath.right(fullPath.size() - (fullPath.lastIndexOf("/")+1));

        //whereTo consist of...
        //[rootPath/] >    [/1/buildCompressed/42]    /[/last few names]
        QString unchangedDir = fullPath;
        QString removeThis = fullPath.mid(0, pathOfRoot.size()+1);
        QString directoryTo = unchangedDir.remove(0,removeThis.size());

        for(int de = 0; de< depth; de++){
            //remove the slashes for directories and the directories until it has
            //only the directory path structure of the file itself
            directoryTo.remove(0,directoryTo.indexOf("/")+1);
        }


        directoryTo.prepend(whereTo);
        copyFile(fullPath, directoryTo, fileName);

    }


}

void DirectoryHandler::copyFile(QString filePath, QString directoryTo, QString fileName){
    //firstly check if that file already exist
    //if so delete it...
    QFile fileExist(directoryTo);
    QDir directoryOfIt(directoryTo);

    //firstly look if there exist a directory with that path
    if(directoryOfIt.exists()){
        QDir().remove(directoryTo);
    }
    else //look if there exist a file with that name and directory
    if(fileExist.exists()){
        //remove if that file exists
        if(!fileExist.remove()){};
            //qDebug()<<" not removed== "<<directoryTo;
    }

    //create the path to where the file must be located
    QDir().mkpath(directoryTo.mid(0, (directoryTo.size()-fileName.size())));

    //copy the file
    QFile().copy(filePath, directoryTo);
    QFile().waitForBytesWritten(-1);

    //finally delete the file that was copied
    QFile().remove(filePath);
}


bool DirectoryHandler::removeDir(const QString &dirName){
    bool result = true;
        QDir dir(dirName);

        if (dir.exists(dirName)) {
            Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
                if (info.isDir()) {
                    result = removeDir(info.absoluteFilePath());
                }
                else {
                    result = QFile::remove(info.absoluteFilePath());
                }

                if (!result) {
                    return result;
                }
            }
            result = dir.rmdir(dirName);
        }

        return result;
}
