#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <Build.h>

/*
 * NOTE: This class is stll in beginner stages and still has some issues
 *
 * Current issues:   
 *      * Location of 7zip console application (7z)
 *      * Linux code
 *
 */

/**
* @class compression
* @brief This class receives a list of files and handles its compression using 7zip
*/
class compression
{
public:
    /**
    * \fn compression()
    * @brief constructor which assigns a build internally
    */
    compression();
    /**
    * \fn compress(QStringList dirs, QString toDir, QString name)
    * @brief compress all files in <dirs> using 7zip and stores a 7z file with name <name> at location <toDir>
    */
    void compress(QStringList dirs, QString toDir, QString name);
    /**
    * \fn compress(QString fromDir, QString toDir, QString name)
    * @brief decompress a 7z file located at <fromDir> with name <name> using 7zip and extracts at location <toDir>
    */
    void decompress(QString fromDir, QString toDir, QString name);
};

#endif // COMPRESSION_H
