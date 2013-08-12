/**
* @file myDirIterator.h
* @brief This is the customised DirIterator class
*/

#ifndef MYDIRITERATOR_H
#define MYDIRITERATOR_H

#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QString>
#include <QVector>

/**
 * @class myDirIterator
 * @brief The myDirIterator class
 */
class myDirIterator{
private:
    /**
     * @var fileNames
     * @brief is a QString vector of the file names
     */
    QVector<QString> fileNames;
    /**
     * @var fileMods
     * @brief is a QString vector of the file modification dates
     */
    QVector<QString> fileMods;
public:
    /**
     * @fn myDirIterator(QString loc);
     * @brief myDirIterator's constructor
     * @param loc the directory passed through
     */
    myDirIterator(QString loc);
    /**
     * @fn ~myDirIterator();
     * @brief the destructor
     */
    ~myDirIterator();

    /**
     * @fn void getFileInfo();
     * @brief iterates throuh the files and saves the relevant information
     */
    void getFileInfo();
    /**
     * @fn vector<QString> retrieveFileNames();
     * @brief retrieveFileNames
     * @return fileNames The file names
     */
    QVector<QString> retrieveFileNames(){return fileNames;}
    /**
     * @fn vector<QString> retrieveFileMods();
     * @brief retrieveFileMods
     * @return fileMods The file modification dates
     */
    QVector<QString> retrieveFileMods(){return fileMods;}

    /**
     * @var it
     * @brief A pointer to a QDirIterator
     */
    QDirIterator *it;
};

#endif // MYDIRITERATOR_H
