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
     * \fn void generateAllMD5(Build build);
     * @brief this generates the list of files and md5's linked to them
     * @param build The build for which the MD5 sum values are generated
     */
    void generateAllMD5(Build build);    
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

signals:

public slots:

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

};

#endif // BUILDMD5_H
