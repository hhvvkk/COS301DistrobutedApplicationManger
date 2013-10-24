#ifndef WATCHER_H
#define WATCHER_H

#include <QObject>
#include <QVariantMap>
#include <QFileSystemWatcher>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QMutex>

class SynchClass;

/**
  * @class Watcher
  * @brief A simple class linked to the builds which will contain all the code the watcher needs in order to resynch the builds at the right time interval
  */
class Watcher : public QObject
{
    Q_OBJECT
signals:
    /**
      * \fn void mustResynchBuild(int buildID);
      * @brief Signal emitted once the build is ready to be resynchronised(happens when the timer runs out) on all machines
      * @param buildID the ID of the build that must be resynchronised
      */
    void mustResynchBuild(int buildID);
    /**
      * \fn void buildTimerCount(int buildID, int timeRemaining);
      * @brief Signal emitted when the timer counts down and there exist a certain amount of time remaining before the resynchronise will be called
      * @param buildID the ID of the build that must soon be resynchronised
      * @param timeRemaining The amount of time remaining on the timer before the build will be resynchronised
      */
    void buildTimerCount(int buildID, int timeRemaining);
public:
    explicit Watcher(QObject *parent = 0);

    /**
      * \fn void addBuildPath(int buildID, QString path);
      * @brief Adds the path to the checklist in order to synchronise the builds at the right time
      * @param buildID The ID of the build that will be checked for changes
      */
    void addBuildPath(int buildID, QString path);

    /**
      * \fn void addBuildPath(int buildID);
      * @param buildID The ID of the build that will be checked for changes
      * @brief removes the build from the watch list
      */
    void removeBuildPath(int buildID);

    /**
      * \fn void stopCountDown(int buildID);
      * @param buildID The ID of the build that might have synchronised but must be stopped
      * @brief Stops the countdown for synchronisation if it occurs
      */
    void stopCountDown(int buildID);

    /**
      * \fn void changeDirToWatch(int buildID, QString newPath);
      * @param buildID The ID of the build that will be synchronised
      * @param newPath The new path that must be watched for changes
      * @brief Change the directory where the watcher is SyncClass is looking at
      */
    void changeDirToWatch(int buildID, QString newPath);

private slots:
    /**
      * \fn void subClassEcho(SynchClass *theClass);
      * @brief A slot connected such that each time the SynchClass echos this function is called
      * @param theClass The class that has emitted the signal
      * */
    void subClassEcho(SynchClass *theClass);

private:
    //A map with the ID of builds to(SynchClass {directory, buildTimer, currentTime })
    QMap<int, SynchClass*> buildMap;
};

/**
  * @class SynchClass
  * @brief An small class to store the variables for the synchronisation
  */
class SynchClass : public QObject{
    Q_OBJECT
signals:
    /**
      * \fn void subClassEcho(SynchClass *thisClass);
      * @brief Signal that is used to signal an echo for this class
      * @param thisClass This class that has emitted the signal
      * */
    void subClassEcho(SynchClass *thisClass);
public:
    QString path;
    QTimer *echoTimer;
    int buildID;
    QTime *time;

    QFileSystemWatcher *directoryWatcher;

    ~SynchClass();
public slots:
    /**
      * \fn void subClassEcho(SynchClass *thisClass);
      * @brief A slot connected such that once the timer timesout this is called
      * */
    void echo();

    /**
      * \fn void fileChanged(QString fileThatChanged);
      * @brief A slot connected such that each time a file changes in the build, this function is called
      * @param fileThatChaned The file that changed
      * */
    void fileChanged(QString fileThatChanged);

private:
    /**
      *
      * \fn timerHeartBeat();
      * @brief A function to call to reset the timer each time a file changes to keep the resynchronising from occuring until change is done
      */
    void timerHeartBeat();


};

#endif // WATCHER_H
