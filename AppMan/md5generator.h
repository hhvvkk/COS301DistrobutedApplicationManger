#ifndef MD5GENERATOR_H
#define MD5GENERATOR_H

#include "md5thread.h"
#include "myDirIterator.h"

#include <QThread>
#include <QMutex>
#include <QCryptographicHash>

class md5Thread;
/**
  * @class md5generator
  * @brief This class will manage and start the threads which will generate md5 values.
  */
class md5generator: public QObject {
    Q_OBJECT

public:
    /**
     * \fn md5generator(QString directory, int threadCnt);
     * @brief constructs the md5generator with directory and thread count
     * @param directory directory for which the md5 is generated
     * @param threadCnt number of threads that will be started
     */
    md5generator(QString directory, int threadCnt);
    /**
     * \fn ~md5generator();
     * @brief The destructor for the md5generator class
     */
    ~md5generator();
    /**
     * \fn void generate();
     * @brief this function will split all files in <directory> and hand them off to each thread for calcutation. It will wait until each thread is complete.
     */
    void generate();
    /**
     * \fn QStringList* getDirs()
     * @brief returns buildFiles
     */
    QStringList* getDirs() { return buildFiles; }
    /**
     * \fn QStringList* getMd5s()
     * @brief returns buildFilesMD5
     */
    QStringList* getMd5s() { return buildFilesMD5; }
    /**
     * \fn QString getDirectoryMD5()
     * @brief returns directoryMD5 which is the MD5 sum of the cmplete directory
     */
    QString getDirectoryMD5() { return directoryMD5; }
    /**
     * \fn void patchThreads(QStringList* dirs, QStringList* dirsMD5, QByteArray mainHash);
     * @brief this function will be called by the threads and will put together all the calculated md5 sums
     * @param dirs The directory paths od the md5 sums that was calculated
     * @param dirsMD5 The md5 sums that was calculated
     * @param mainhash Contains the partial md5 sum of the complete directory
     */
    void patchThreads(QStringList* dirs, QStringList* dirsMD5, QByteArray mainHash);

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
      * @var directoryMD5 The md5 sum of the directorys
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
};

#endif // MD5GENERATOR_H
