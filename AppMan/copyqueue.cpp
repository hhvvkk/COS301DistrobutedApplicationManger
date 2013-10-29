#include "copyqueue.h"

CopyQueue::CopyQueue(QObject *parent) :
    QObject(parent)
{
    queue = new QQueue<CopierPhysical*>();
    isFinished = false;
    isStarted = false;
}


CopyQueue::~CopyQueue(){
    while(!queue->isEmpty()){
        queue->dequeue();
        //the copierphysical deletes itself
        //cp->deleteLater();
    }
    delete queue;
}

bool CopyQueue::append(CopierPhysical *physicalCopier){
    lock.lock();

    //if it is finished, it means the queue is about to delete
    if(isFinished){
        //nb unlock...
        lock.unlock();
        return false;
    }

    //else the queue will be able to contiue
    queue->enqueue(physicalCopier);
    connect(physicalCopier, SIGNAL(copierPhysicalDone(int)), this, SLOT(popFront(int)));
    lock.unlock();

    return true;
}

void CopyQueue::popFront(int BuildID){
    lock.lock();
    if(!queue->isEmpty()){
        CopierPhysical *cp = queue->dequeue();
        if(BuildID != cp->getBuildID())
            qDebug()<<"DequeueError:(BuildID)"<<BuildID;
        cp->deleteLater();
    }
    if(!queue->isEmpty()){
        CopierPhysical *physicalCopier = queue->front();
        int port = physicalCopier->startServer();
        int BuildID = physicalCopier->getBuildID();
        emit nextInQueue(port, BuildID);
    }else{
        //else it is empty
        isFinished = true;
        emit queueFinished(this);
    }
    lock.unlock();
}

void CopyQueue::startCopying(){
    lock.lock();
    if(!isStarted){
        isStarted = true;
        //which means it requires the physical copy over process to begin
        //start this by starting the first(head) item
        CopierPhysical *physicalCopier = queue->front();
        int port = physicalCopier->startServer();
        int BuildID = physicalCopier->getBuildID();
        emit nextInQueue(port, BuildID);
    }
    //else do nothing...it is not needed
    lock.unlock();
}
