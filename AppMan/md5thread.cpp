#include "md5thread.h"

#include <QFile>
#include <QDir>
#include <QCryptographicHash>

md5Thread::md5Thread(int id, QStringList* list, BuildMD5* r){
    threadNumber = id;
    dirs = list;
    reply = r;
}

md5Thread::~md5Thread(){
    delete dirs;
    delete reply;
}

void md5Thread::run() {    
    QStringList* dirsMD5 = new QStringList();
    QCryptographicHash md5(QCryptographicHash::Md5);
    QCryptographicHash md5full(QCryptographicHash::Md5);

    for (int i=0;i<dirs->size();i++) {
        QFile file(dirs->at(i));
        file.open(QFile::ReadOnly);
        QByteArray ba = file.readAll();
        md5.addData(ba);
        md5full.addData(ba);
        QString hash(md5.result().toHex().constData());
        dirsMD5->append(hash);
        file.close();
        md5.reset();
    }        
    reply->patchThreads(threadNumber, dirs,dirsMD5,md5full.result());
    this->terminate();
}
