#ifndef COPYCOMPARE_H
#define COPYCOMPARE_H

#include <QObject>
#include <QStringList>
#include <QDebug>

/**
  * @class CopyCompare
  * @brief This class is used to determine which files are different from master to slave
  */
class CopyCompare : public QObject
{
    Q_OBJECT
public:
    /**
      * \fn CopyCompare(QObject *parent = 0);
      * @brief Constructor
      */
    explicit CopyCompare(QObject *parent = 0);


    /**
      * \fn
      * @brief Destructor
      */
    ~CopyCompare();

    /**
      * \fn void compareMD5(const QString *masterMD5Value, const QString *masterFilePath, QString slaveMd5Value);
      * @brief This function simply returns the filePaths variable
      * @return Returns the file paths to be copied over
      */
    QStringList getFilepaths();

    /**
      * \fn void compareMD5(const QString *masterMD5Value, const QString *masterFilePath, QString slaveMd5Value);
      * @brief A function which compares whether or not the file should be included in the build and adds it to the filePaths variable if it should be included(executed when the file exist on both slave and master)
      * @param masterMD5Value The MD5sum value on the master machine
      * @param masterFilePath The path to the file on the master machine
      * @param slaveMd5Value The MD5sum value on the slave machine
      */
    void compareMD5(const QString *masterMD5Value, const QString *masterFilePath, QString slaveMd5Value);

    /**
      * \fn void createFile(QString fileDirectory);
      * @brief A function simply adding the file to the list of files to copy over
      * @param fileDirectory The directory where the file is
      */
    void createFile(QString fileDirectory);

    /**
      * \fn void createFile(QString fileDirectory);
      * @brief A function which calculates where the synchronization is currently
      * @return Returns the amount where the synchronisation process is
      */
    double percentageSynched();

    /**
      * \fn void createFile(QString fileDirectory);
      * @brief This function simply adds the file to be deleted in the list of delete file paths
      * @param file The file to be deleted
      */
    void deleteFile(const QString file);

    /**
      * \fn void createFile(QString fileDirectory);
      * @brief A helper function returning the json string which must be sent to the client in order to delete the files that should not be there
      * @return Returns the json string which will be sent to the client
      * @param BuildID The ID of the build for which the delete json String is obtained(must be included in the delete json string)
      */
    QString getDeleteJsonString(QString BuildID);

    /**
      * \fn bool isSynchronised();
      * @brief A function which will check if there are files required to be sent to the slave machine
      * @return Return true if there are no files to copy over or delete and false if there exist files to delete or copy over
      */
    bool isSynchronised();
private:
    unsigned long int numFilesNotCopied;
    QStringList *filePaths;
    QStringList *deleteFilePaths;

};

#endif // COPYCOMPARE_H
