#ifndef COPYQUEUE_H
#define COPYQUEUE_H

#include <QObject>
#include <QQueue>
#include <QMutex>

#include "copierphysical.h"


/**
  * @class CopyQueue
  * @brief This class is to allow the copies to automatically be called by making use of a queue
  */
class CopyQueue : public QObject
{
    Q_OBJECT
signals:
    /**
      * \fn void queueFinished();
      * @brief The signal emitted when the queue is empty
      * @param thisQueue The queue that emitted the signal will indicate that it is done so that it can be deleted
      */
    void queueFinished(CopyQueue *thisQueue);

    /**
      * \fn void nextInQueue(int port, int BuildID);
      * @brief This signal will be emitted each time the next server in the queue has started and waiting transferal of a build zip
      */
    void nextInQueue(int port, int BuildID);

public:
    explicit CopyQueue(QObject *parent = 0);

    ~CopyQueue();

    /**
      * \fn void append(CopierPhysical *copier);
      * @brief Adds the physical copier to the queue to be executed at a later stage for physical copy transfer
      * @return Returns true if it was successful in adding the copier to the queue and false otherwise
      */
    bool append(CopierPhysical *copier);


    /**
      * \fn void startCopying();
      * @brief A function which starts the physical servers to start copying if it does not already
      */
    void startCopying();
private slots:

    /**
      * \fn void popFront(int BuildID);
      * @brief A function which will take the next physical copier in the queue and start the physical copy over process
      * @param BuildID The build ID which will be popped from the queue
      */
    void popFront(int BuildID);

private:
    /**
      * A queue containing physical copiers to be copied over next
      */
    QQueue <CopierPhysical*>*queue;

    /**
      * A boolean to indicate that this copy queue will be deleted and will not be able to add new physical copy instructions
      */
    bool isFinished;


    /**
      * A boolean indicating whether or not the physical copy process has been started
      */
    bool isStarted;

    QMutex lock;

};

#endif // COPYQUEUE_H
