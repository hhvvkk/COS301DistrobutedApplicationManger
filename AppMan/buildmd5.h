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
     * @param dirs The directory paths of the md5 sums that was calculated
     * @param dirsMD5 The md5 sums that was calculated
     * @param mainhash Contains the partial md5 sum of the complete directory
     */
    void patchThreads(QStringList* dirs, QStringList* dirsMD5, QByteArray mainHash);

    /**
     * \fn QString getCurrentBuildDirectory()
     * @brief returns the build directory at currentIndex
     */
    const QString* getCurrentBuildDirectory() const;
    /**
     * \fn QString getCurrentFileMd5Sum()
     * @brief returns the md5 sum of a build directory at currentIndex
     */
    const QString* getCurrentFileMd5Sum() const;
    /**
     * \fn QString next()
     * @brief advances currentIndex by 1
     */
    void next();
    /**
     * \fn QString getCurrentIndex()
     * @brief returns currentIndex
     */
    int getCurrentIndex();
    /**
     * \fn QString getSize()
     * @brief returns how meany directories are in buildFiles
     */
    int getSize();

private:
    /**
      * @var threads A list of handles for all running threads
      **/
    QVector<md5Thread*> threads;
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
      * @var currentIndex Maintains the position of which directory is currently used
      **/
    int currentIndex;
};

#endif // BUILDMD5_H
