#ifndef PROTOCOPYOVER_H
#define PROTOCOPYOVER_H

#include "protocol.h"

class ProtoCopyOver : public Protocol
{
public:
    /**
     * \fn ProtoCopyOver(QObject *parent = 0);
     * @brief the constructor
     */
    ProtoCopyOver(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket);
     * @brief The function that will handle all the ConnectProtocol functions
     * @param data the data to be parsed
     * @param man the management to perform functions on
     * @param slaveSocket the socket if it is needed to write to it
     */
    void handle(QString data, Management *man, QTcpSocket *slaveSocket);

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
    void GotABuild(QString data, Management *man, QTcpSocket *slaveSocket);

    /**
     * \fn void sizeCheckCertainBuild(QString buildNo, QTcpSocket *slaveSocket);
     * @brief The function which will enable the size check of only one build
     * @param buildNo the Id of the build to copy over
     * @param slaveSocket the socket of client to size check on the buildNo
     */
    void sizeCheckCertainBuild(QString buildNo, QTcpSocket *slaveSocket);

};

#endif // PROTOCOPYOVER_H
