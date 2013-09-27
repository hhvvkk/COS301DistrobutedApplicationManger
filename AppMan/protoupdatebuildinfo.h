#ifndef PROTOUPDATE_H
#define PROTOUPDATE_H

#include "protocol.h"




/**
  * @class ProtoUpdateBuildInfo
  * @brief This class is the protocol class handling the update of build information to the slave machine
  */
class ProtoUpdateBuildInfo : public Protocol
{
    Q_OBJECT
public:

    /**
     * \fn ProtoUpdate(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoUpdateBuildInfo(QObject *parent = 0);


    /**
     * \fn ProtoUpdateBuildInfo();
     * @brief the destructor
     */
    ~ProtoUpdateBuildInfo();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the Copy over functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);

};

#endif // PROTOUPDATE_H
