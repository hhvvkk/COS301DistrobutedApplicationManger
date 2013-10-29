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
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket);

    /**
      * \fn void DeleteBuild(QTcpSocket *slavesocket, int buildID);
      * @brief A function to delete the build on the build physically and remove it from the machine
      * @param slaveSocket The socket on which the build will be deleted
      * @param jsonObject A QVariantMap which contains values
      * @param management the management to perform functions on
      */
    void DeleteBuild(QTcpSocket *slaveSocket, QVariantMap jsonObject, Management *management);
};

#endif // PROTODELETEBUILD_H
