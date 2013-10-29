#ifndef PROTOSENDBUILD_H
#define PROTOSENDBUILD_H

#include "protocol.h"
#include <QMutex>

class CopySenderServer;

class Machine;

/**
  * @class ProtoSendBuild
  * @brief This is the protocol for SendBuild(Which will create a server to physically copy the file over the network)
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
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket);


    /**
     * \fn void SizeCheckAllBuildsDone();
     * @brief This function will be invoked once SizeCheckAllBuilds has completed and will resynchronize the builds
     * @param slaveSocket The socket if it is needed to write to it
     */
    void SizeCheckAllBuildsDone(QTcpSocket *slaveSocket,  Management *management);

    /**
      * \fn void sizeCheckCertainBuildDone(QTcpSocket *slaveSocket);
      * @brief The following invokes the check for all builds to see whether their sizes match
      */
    void sizeCheckCertainBuildDone(/*int buildID, Machine *machine, Management *management,*/ QTcpSocket *slaveSocket);

    /**
      * \fn void SizeCheckAllBuilds();
      * @brief The following invokes the check if of all build sizes and is used to create a loop to continually update the build until the information is updated.
      */
    void SizeCheckAllBuilds();


private slots:
    /**
      * \fn void copySenderServerDone(CopySenderServer * deleteCopySender);
      * @brief A function deleting a copy sender server which was used to synchronise multiple builds
      * @param deleteCopySender The CopySenderServer to delete
      */
    void copySenderServerDone(CopySenderServer * deleteCopySender);




    /**
      * \fn void fullySynchronisedBuild(int intBuildID, int machineId, Management *management);
      * @brief Executes when a build is fully synchronised on slave machine(i.e. no files to copy and no files to delete)
      * @param intBuildID the ID of the build which is fully synchronised
      * @param machineId The ID of the machien which has a full synchronisation
      * @param management The management class which will be notified of the syncrhonisation
      */
    void fullySynchronisedBuild(int intBuildID, int machineId, Management *management);

private:

    /**
      * \fn void initiateGetBuildStructure(int buildID);
      * @brief A function to transfer the directory structure of the build over the network such that it is completely synchronised
      * @param buildID The ID for which the client must initiate the transferral of the directory structure
      * @param slaveSocket The slave socket that will be used to initiate the ProtoStructure
      */
    void initiateGetBuildStructure(int buildID, QTcpSocket * slaveSocket);


    /**
      * \fn void removeCopySenderServer(CopySenderServer * deleteCopySender);
      * @brief A which will be called concurrently by copySenderServerDone to remove the copySenderServer
      * @param removeThis The CopySenderServer to be deleted
      */
    void removeCopySenderServer(CopySenderServer * removeThis);

private:
    QList <CopySenderServer*>sendList;

    ProtoSendBuild *thisPointer;
};

#endif // PROTOSENDBUILD_H
