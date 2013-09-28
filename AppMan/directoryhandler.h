#ifndef DIRECTORYHANDLER_H
#define DIRECTORYHANDLER_H

#include <QObject>
#include <QDir>
#include <QFile>
#include <QDebug>

/**
 * @class DirectoryHandler
 * @brief Handles directory related aspects such as copy files from one directory to another keeping structure and deletions of files
 */
class DirectoryHandler : public QObject
{
    Q_OBJECT
public:
    explicit DirectoryHandler(QObject *parent = 0);

    /**
     * @brief A function whereby all the files from a directory are added to the list passed in as a parameter
     * @param d The directory where the list will be populated from
     * @param list The list to which file paths are added
     */
    static void recurseAddDir(QDir d, QStringList & list) ;

    /**
     * @brief A function to copy all files from the list over to the directory(WhereTo) while removing the necesssary depth of directories and keeping structure
     * @param depth The depth which will be removed(the amount of directories to remove before concatenation of paths)
     * @param list The list of files to copy over recusively
     * @param whereTo The directory where all the files will be copied to
     * @param pathOfRoot The root path where the files are located to be copied over
     */
    static void copyOverFromList(int depth, QStringList &list, QString whereTo, QString pathOfRoot);

    /**
     * @brief A function to handle the physical copy of file over from filePath to the specified directory
     * @param filePath The path of the file to be copied
     * @param directoryTo The directory where the file will be copied including the name of file
     * @param FileName The name of the file which will be copied
     */
    static void copyFile(QString filePath, QString directoryTo, QString FileName);

    /**
     * @brief A function to remove a directory along with all of its contents
     * @param dirName The Directory name that will be removed along all its content
     * @return Return true if it succeeded in removing the directory
     */
    static bool removeDir(const QString &dirName);
};

#endif // DIRECTORYHANDLER_H
