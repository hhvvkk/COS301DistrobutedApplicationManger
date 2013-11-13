#include "simthread.h"

simThread::simThread(QObject *parent, QString build, QString recArg, Management * man){
    stBuild = build;
    stArg = recArg;
    management = man;
}

void simThread::run(){
    QMap<QString,QString> appList = management->getAppList();
    QString build = stBuild;
    QString recArg = stArg;
    QString runner = "";
        if(build.contains("App")){
            QString appName = build;

            appName.remove(0,4);
            QMapIterator<QString, QString> m(appList);
            while(m.hasNext()){
                m.next();
                if(m.key().compare(appName)==0){
                    runner = m.value();
                }
            }

        }
        QString app;
        QString arg;
        if(runner.compare("") == 0){
            build.chop(build.length() - build.indexOf("-"));
            Build *b = management->getBuildByID(build.toInt());
            app = /*"\"\"C:/AppManClient\"/"+*/b->getBuildDirectory();
            arg = "/"+recArg;
            QProcess* p = new QProcess();
            p->start(app+arg);
            p->waitForFinished(-1);
            p->kill();
            p->deleteLater();
        }else{
            app = "\"\""+runner+"\"\"";
            arg = "\" \""+recArg+"\"\"";

            QString data = app + arg;
            char * gg = data.toUtf8().data();

            system(gg);
        }
        this->terminate();
}

simThread::~simThread(){
    management->deleteLater();
}
