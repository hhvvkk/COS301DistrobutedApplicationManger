#ifndef PROTODELETEBUILD_H
#define PROTODELETEBUILD_H


#include "protocol.h"

/**
  * @class ProtoDeleteBuild
  * @brief This class is the protocol class handling protocols for deletion of builds
  */
class ProtoDeleteBuild : public Protocol
{
    Q_OBJECT
public:

    /**
     * \fn ProtoDeleteBuild(QObject *parent);
     * @brief The constructor
     */
    explicit ProtoDeleteBuild(QObject *parent);

    /**
      * \fn ~ProtoDeleteBuild();
      * @brief The destructor
      */
    ~ProtoDeleteBuild();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the delete protocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param management the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket);

    /**
      * \fn void deleteBuildFromSlave(QTcpSocket *slavesocket, int buildID);
      * @brief A function to delete the build on the slave machine whereby the slave is notified to delete it
      * @param slaveSocket The socket on which the build will be deleted
      * @param buildID The ID of the build which will be deleted
      */
    void deleteBuildFromSlave(QTcpSocket *slaveSocket, int buildID);

    /**
      * \fn void DeleteSuccess(QVariantMap jsonObject, Management *man);
      * @brief Executes each time there is a build on the slave which has been sucessfully deleted
      * @param man the management to perform functions on
      * @param jsonObject A QVariantMap which contains values
      */
    void DeleteSuccess(QVariantMap jsonObject, Management *man);
};

#endif // PROTODELETEBUILD_H
