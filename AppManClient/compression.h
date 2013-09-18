#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <Build.h>
#include <QObject>


/**
* @class Compression
* @brief This class receives a list of files and handles its compression using 7zip
*/
class Compression : public QObject{
    Q_OBJECT
public:
    /**
    * \fn compression()
    * @brief constructor which assigns a build internally
    */
    Compression(QObject *par = 0);
    /**
    * \fn compress(QStringList dirs, QString toDir, QString name)
    * @brief compress all files in <dirs> using 7zip and stores a 7z file with name <name> at location <toDir>
    * @param dirs Contains a list of all directory and file paths that needs to be compressessed
    * @param toDir The path to the newly compressed file
    * @param name Specifies the name of the newly compressed file
    */
    void compress(QStringList dirs, QString toDir, QString buildDirectory);

    /**
    * \fn decompress(QString fromDir, QString toDir, QString name)
    * @brief decompress a 7z file located at <fromDir> with name <name> using 7zip and extracts at location <toDir>
    * @param fromDir The directory path to the compressed file that needs to be decompressed
    * @param toDir The directory path to the newly decompressed file
    * @param name Specifies the name of the newly decompressed file
    */
    void decompress(QString zipPath, QString toDir);

    bool zipInTact(QString directoryOfZip);

};

#endif // COMPRESSION_H
