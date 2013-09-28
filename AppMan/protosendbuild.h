#ifndef PROTOSENDBUILD_H
#define PROTOSENDBUILD_H

#include "protocol.h"
#include <QMutex>

class CopySenderServer;

class Machine;

/**
  * @class ProtoSendBuild
  * @brief This is the protocol for SendBuild
  */
class ProtoSendBuild : public Protocol
{
    Q_OBJECT
public:
    /**
     * \fn ProtoSendBuild(QObject *parent = 0);
     * @brief The constructor
     */
    ProtoSendBuild(QObject *parent = 0);

    /**
      * \fn ~ProtoSendBuild();
      * @brief The destructor for the protocol
      */
    ~ProtoSendBuild();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the SendBuild protocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);


    /**
     * \fn void SizeCheckAllBuildsDone();
     * @brief This function will be invoked once SizeCheckAllBuilds has completed and will resynchronize the builds
     * @param slaveSocket The socket if it is needed to write to it
     */
    void SizeCheckAllBuildsDone(QTcpSocket *slaveSocket,  Management *management);

    void sizeCheckCertainBuildDone(/*int buildID, Machine *machine, Management *management,*/ QTcpSocket *slaveSocket);

    /**
      * \fn void SizeCheckAllBuilds();
      * @brief The following invokes the checkinf of all build sizes and is used to create a loop to continually update the build until the information is updated.
      */
    void SizeCheckAllBuilds();
private slots:
    void copySenderServerDone(CopySenderServer * deleteCopySender);

private:
    /**
      * A lock to prevent multiple CopySenderServers from being created and prevent
      * CopySenderServers which are being deleted from being used
      */
    QMutex *lock;

    QList <CopySenderServer*>sendList;

    ProtoSendBuild *thisPointer;
};

#endif // PROTOSENDBUILD_H
