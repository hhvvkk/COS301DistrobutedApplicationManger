#ifndef BUILDMD5_H
#define BUILDMD5_H

#include "md5thread.h"
#include "myDirIterator.h"

#include <QThread>
#include <QMutex>
#include <QCryptographicHash>

class md5Thread;
/**
  * @class BuildMD5
  * @brief This class will manage and start the threads which will generate md5 values.
  */
class BuildMD5 : public QObject {
    Q_OBJECT

public:
    /**
     * \fn md5generator(QString directory, int threadCnt);
     * @brief constructs the md5generator with directory and thread count
     * @param directory directory for which the md5 is generated
     * @param threadCnt number of threads that will be started
     */
    BuildMD5(QString directory, int threadCnt);
    /**
     * \fn ~md5generator();
     * @brief The destructor for the md5generator class
     */
    ~BuildMD5();
    /**
     * \fn void generate();
     * @brief this function will split all files in <directory> and hand them off to each thread for calcutation. It will wait until each thread is complete.
     */
    void generate();
    /**
     * \fn QStringList* getBuildFiles()
     * @brief returns buildFiles
     */
    QStringList* getBuildFiles() { return buildFiles; }
    /**
     * \fn QStringList* getBuildFilesMD5()
     * @brief returns buildFilesMD5
     */
    QStringList* getBuildFilesMD5() { return buildFilesMD5; }
    /**
     * \fn QString getDirectoryMD5()
     * @brief returns directoryMD5 which is the MD5 sum of the cmplete directory
     */
    QString getDirectoryMD5() { return directoryMD5; }
    /**
     * \fn void patchThreads(QStringList* dirs, QStringList* dirsMD5, QByteArray mainHash);
     * @brief this function will be called by the threads and will put together all the calculated md5 sums
     * @param threadNumber The thread number of current returning thread
     * @param dirs The directory paths of the md5 sums that was calculated
     * @param dirsMD5 The md5 sums that was calculated
     * @param mainhash Contains the partial md5 sum of the complete directory
     */
    void patchThreads(int threadNumber, QStringList* dirs, QStringList* dirsMD5, QByteArray mainHash);

    /**
     * \fn QString getCurrentBuildDirectory(int i)
     * @brief returns the build directory at i
     * @param i The index of the directory to get
     */
    const QString* getCurrentBuildDirectory(int i) const;
    /**
     * \fn QString getCurrentFileMd5Sum(int i)
     * @brief returns the md5 sum of a build directory at currentIndex
     */
    const QString* getCurrentFileMd5Sum(int i) const;

    /**
     * \fn QString getSize()
     * @brief returns how meany directories are in buildFiles
     */
    int getSize();

    /**
     * \fn void setIsOld(bool value = true);
     * @brief sets whether this buildMD5 is old or not
     * @param value The value to which the isOld will be set
     */
    void setIsOld(bool value = true);

    /**
     * \fn bool isOld();
     * @brief returns how meany directories are in buildFiles     *
     * @return Returns true if this buildMD5 class is old, and false if it is not
     */
    bool isOld();

    /**
     * \fn void setIsInUse(bool value);
     * @brief sets whether this buildMD5 is used or not
     * @param value The value to which the isOld will be set
     */
    void setIsInUse(bool value);

    /**
     * \fn void tryDelete();
     * @brief Tries to delete this class, but will not if there exists other entities still using this clas
     */
    void tryDelete();
private:
    /**
      * @var threads A list of handles for all running threads
      **/
    QVector<md5Thread*> threads;
    /**
      * @var md5perthread A list of the total md5values return by specific thread
      **/
    QByteArray* md5perthread;
    /**
      * @note buildFiles and buildFilesMD5 are linked through indices.
        E.g. buildFiles[1] {filepath} has a MD5 sum value of buildFilesMD5[1]
      */
    /**
      * @var buildFiles The filepath for each of the files
      **/
    QStringList* buildFiles;
    /**
      * @var buildFilesMD5 The md5 sum for each of the files
      **/
    QStringList* buildFilesMD5;

    /**
      * @var dir The directory which need to be calculated
      **/
    QString dir;
    /**
      * @var directoryMD5 The md5 sum of the directories
      **/
    QString directoryMD5;
    /**
      * @var threadCount The number of threads that will be created
      **/
    int threadCount;
    /**
      * @var finished An indicator to see if all threads have finished
      **/
    int finished;
    /**
      * @var lock Ensures mutual exclusivity for funtion patchThreads
      **/
    QMutex* lock;

    /**
      * @var dirHash The md5 hasher which will calcutate the total md5 of directory
      **/
    QCryptographicHash* dirHash;

    /**
      * @var old A boolean which indicates if this class is outdated or not
      **/
    bool old;

    /**
      * @var inUse A boolean showing if the buildMD5 class is being used or not
      **/
    bool inUse ;


    /**
      * @var deleteLock A lock that ensures that the user amount is increased as it should be
      **/
    QMutex deleteLock;

    /**
      * @var users Showing the amount of entities using this
      **/
    int users;
};

#endif // BUILDMD5_H
