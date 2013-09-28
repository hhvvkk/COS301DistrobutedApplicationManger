#ifndef MD5THREAD_H
#define MD5THREAD_H

#include "md5generator.h"

#include <QStringList>
#include <QThread>

class md5generator;
/**
 * @class md5Thread
 * @brief This class is a thread that will calculate the md5 values of a set of files
 */
class md5Thread : public QThread {
    Q_OBJECT

public:
    /**
     * \fn md5Thread(QStringList* list, md5generator* r);
     * @brief constructs the md5 thread with a fileset and a pointer to the generator
     * @param list pointer to the list of files that needs to be md5 calculated
     */
    md5Thread(QStringList* list, md5generator* r);
    /**
     * \fn ~md5Thread();
     * @brief destructor
     */
    ~md5Thread();
    /**
     * \fn void run();
     * @brief overloaded run function which executes when the thread starts. Does the md5 calculations.
     */
    void run();

private:
    /**
      * @var dirs list of file paths for which each md5 needs to be calculated
      **/
    QStringList* dirs;
    /**
      * @var reply pointer back to the generator who started this thread.
      **/
    md5generator* reply;
};

#endif // MD5THREAD_H