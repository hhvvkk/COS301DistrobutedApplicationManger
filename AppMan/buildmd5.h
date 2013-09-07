#ifndef BUILDMD5_H
#define BUILDMD5_H

#include <QObject>
#include <QCryptographicHash>
#include "Build.h"
#include "myDirIterator.h"

/**
  * @class BuildMD5
  * @brief This class will generate the MD5 sum values for each of the files inside build directory
  */
class BuildMD5 : public QObject
{
    Q_OBJECT
public:
    explicit BuildMD5(QObject *parent = 0);

    /**
     * \fn ~BuildMD5();
     * @brief The destructor for the BuildMD5 class
     */
    ~BuildMD5();

    /**
     * \fn void generateAllMD5(Build build);
     * @brief this generates the list of files and md5's linked to them
     * @param dir The directory for which the md5 is generated
     */
    void generateAllMD5(QString directory);

    /**
     * @fn QStringList* getBuildFiles();
     * @brief retrieves buildFiles
     * @return buildFiles
     */
    QStringList* getBuildFiles(){return buildFiles;}

    /**
     * @fn QStringList* getBuildFilesMD5();
     * @brief retrieves buildFilesMD5
     * @return buildFilesMD5
     */
    QStringList* getBuildFilesMD5(){return buildFilesMD5;}

    const QString* getCurrentBuildDirectory() const;

    const QString* getCurrentFileMd5Sum() const;

    void next();

    int getCurrentIndex();

    int getSize();

private:
    /**
      * @note buildFiles and buildMD5 are linked through indices.
        E.g. buildFiles[1] {filepath} has a MD5 sum value of buildFilesMD5[1]
      */
    /**
      * @var buildFiles The filepath for each of the files
      **/
    QStringList* buildFiles;
    /**
      * @var buildFilesMD5 The MD5 Checksum for each of the files linked to buildFiles
      **/
    QStringList* buildFilesMD5;

    int currentIndex;

};

#endif // BUILDMD5_H
