#ifndef COPYCOMPARE_H
#define COPYCOMPARE_H

#include <QObject>
#include <QStringList>
#include <QDebug>

/**
  * @class CopyCompare
  * @brief This class is used to determine which files are different from master to build
  */
class CopyCompare : public QObject
{
    Q_OBJECT
public:
    explicit CopyCompare(QObject *parent = 0);

    ~CopyCompare();

    void compareMD5(const QString *masterMD5Value, const QString *masterFilePath, QString slaveMd5Value);

    void createFile(QString fileDirectory);

    double percentageSynched();

    void deleteFile(const QString file);

    QString getDeleteJsonString(QString buildNo);
private:
    unsigned long int numFilesNotCopied;
    QStringList *filePaths;
    QStringList *deleteFilePaths;

};

#endif // COPYCOMPARE_H
