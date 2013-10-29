#include "watcher.h"

Watcher::Watcher(QObject *parent) :
    QObject(parent)
{
}

void Watcher::addBuildPath(int buildID, QString buildPath){
    SynchClass *existingOne = buildMap.value(buildID, 0);

    if(existingOne != 0){
        return;
    }

    QTimer *theEchoTimer = new QTimer();
    QTime *theTime = new QTime(0,0,0,0);
    theEchoTimer->stop();

    //directory is path
    //set the interval to 1 second
    theEchoTimer->setInterval(1000);

    SynchClass *sClass = new SynchClass();

    sClass->echoTimer = theEchoTimer;
    sClass->path = buildPath;
    sClass->buildID = buildID;
    sClass->time = theTime;

    //create the physical directory watcher to keep an eye on the build
    QFileSystemWatcher *fileSystemWatcher = new QFileSystemWatcher();
    connect(fileSystemWatcher, SIGNAL(fileChanged(QString)), sClass, SLOT(fileChanged(QString)));
    connect(fileSystemWatcher, SIGNAL(directoryChanged(QString)), sClass, SLOT(fileChanged(QString)));
    sClass->directoryWatcher = fileSystemWatcher;
    fileSystemWatcher->addPath(buildPath);

    //insert it into the map
    buildMap.insert(buildID, sClass);

    //connect the timer to the function
    //connect(&notifyTimer, SIGNAL(timeout()), this, SLOT(signalNotifyProgress()));
    connect(theEchoTimer, SIGNAL(timeout()), sClass, SLOT(echo()));
    connect(sClass, SIGNAL(subClassEcho(SynchClass*)), this,SLOT(subClassEcho(SynchClass*)));
}

void Watcher::removeBuildPath(int buildID){
    SynchClass *sClass = buildMap.take(buildID);
    sClass->deleteLater();
}

void Watcher::stopCountDown(int buildID){
    SynchClass *sClass = buildMap.value(buildID, 0);

    if(sClass == 0){
        return;
    }

    sClass->echoTimer->stop();
    QTime *newTimer = new QTime(0,0,0,0);
    QTime *oldTimer = sClass->time;
    sClass->time = newTimer;

    delete oldTimer;

    emit buildTimerCount(buildID, 0);

}

void Watcher::changeDirToWatch(int buildID, QString newPath){
    SynchClass *sClass = buildMap.value(buildID, 0);

    if(sClass == 0)
        return;

    emit buildTimerCount(buildID, 0);

    QTime *theTime = sClass->time;
    sClass->echoTimer->stop();
    QTime *newTime = new QTime(0,0,0,0);
    sClass->time = newTime;
    delete theTime;

    QFileSystemWatcher *newFileSystemWatcher = new QFileSystemWatcher();
    newFileSystemWatcher->addPath(newPath);

    connect(newFileSystemWatcher, SIGNAL(fileChanged(QString)), sClass, SLOT(fileChanged(QString)));
    connect(newFileSystemWatcher, SIGNAL(directoryChanged(QString)), sClass, SLOT(fileChanged(QString)));

    sClass->path = newPath;

    QFileSystemWatcher *temp = sClass->directoryWatcher;
    temp->deleteLater();
    temp = 0;

    sClass->directoryWatcher = newFileSystemWatcher;

}

void Watcher::subClassEcho(SynchClass *theClass){
    //get the current time remaining and echo it
    QTime *theTime = theClass->time;

    int elapsedMS = theTime->elapsed();


    //more than 25 seconds
    if(elapsedMS >= 25000){
        //it means the build must be resynchronised since the time ran out
        theClass->echoTimer->stop();
        emit mustResynchBuild(theClass->buildID);
        QTime *newTime = new QTime(0,0,0,0);
        theClass->time = newTime;
        delete theTime;
    }

    int currentTime = elapsedMS/1000;
    currentTime = 25 - currentTime;
    emit buildTimerCount(theClass->buildID, currentTime);
}

SynchClass::~SynchClass(){
    echoTimer->deleteLater();
    directoryWatcher->deleteLater();
    delete time;
}

void SynchClass::fileChanged(QString fileThatChanged){
    timerHeartBeat();
}

void SynchClass::echo(){
    emit subClassEcho(this);
}

void SynchClass::timerHeartBeat(){
    time->restart();
    if(!echoTimer->isActive())
        echoTimer->start();
}
