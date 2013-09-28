#ifndef PROTOCOPYOVER_H
#define PROTOCOPYOVER_H

#include "protocol.h"

/**
  * @class ProtoCopyOver
  * @brief This class is the protocol class handling the logical copy over from master to slave
  */
class ProtoCopyOver : public Protocol
{
    Q_OBJECT
public:
    /**
     * \fn ProtoCopyOver(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoCopyOver(QObject *parent = 0);


    /**
     * \fn ~ProtoCopyOver();
     * @brief The destructor for the protocol
     */
    ~ProtoCopyOver();

    /**
     * \fn void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the Copy over functions
     * @param jsonObject A QVariantMap which contains values
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);

    /**
     * \fn void copyBuildOver(int buildId, QString buildName, QTcpSocket *slaveSocket);
     * @brief The function that execute a copy over so that the slave is notified of new build
     * @param buildId the Id of the build to copy over
     * @param buildName the Name of the build to copy over
     * @param slaveSocket the socket of client to notify with the new build
     */
    void copyBuildOver(int buildId, QString buildName, QTcpSocket *slaveSocket);
signals:

public slots:

private:
    /**
     * \fn void GotABuild(int buildId, QString buildName);
     * @brief A function invoked when the ReadyRead observes 'GotABuild' in the data string and parses it
     * @param data The data to be parsed by this function
     */
    void GotABuild(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);

    /**
     * \fn void sizeCheckCertainBuild(QString BuildID, QTcpSocket *slaveSocket);
     * @brief The function which will enable the size check of only one build
     * @param BuildID the Id of the build to copy over
     * @param slaveSocket the socket of client to size check on the BuildID
     */
    void sizeCheckCertainBuild(QString BuildID, QTcpSocket *slaveSocket);

};

#endif // PROTOCOPYOVER_H
