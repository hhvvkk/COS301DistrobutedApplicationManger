#ifndef PROTOSENDSTRUCTURE_H
#define PROTOSENDSTRUCTURE_H

#include "protocol.h"

/**
  * @class ProtoSendStructure
  * @brief The Protocol Send structure which will duplicate the file structure(e.g. directories) on the client side
  */
class ProtoSendStructure : public Protocol
{
public:
    /**
     * \fn ProtoSendStructure(QObject *parent = 0);
     * @brief The constructor
     */
    ProtoSendStructure(QObject *parent = 0);

    /**
      * \fn ~ProtoSendStructure();
      * @brief The destructor for the protocol
      */
    ~ProtoSendStructure();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the sendStructure protocol functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);

private:
    /**
     * \fn void InitiateGetStructure(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);
     * @brief Asks the master machine for a certain builds structure
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param masterSocket the socket if it is needed to write to it
     */
    void InitiateGetStructure(QVariantMap jsonObject, Management *management, QTcpSocket *masterSocket);

    /**
     * \fn void DuplicateStructure(QVariantMap jsonObject, Management *management, QTcpSocket *slaveSocket);
     * @brief Goes and duplicate the structure that the master sent here
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     */
    void DuplicateStructure(QVariantMap jsonObject, Management *management);
};

#endif // PROTOSENDSTRUCTURE_H
