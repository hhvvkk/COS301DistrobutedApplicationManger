/**
* @file socketclient.h
* @brief This is the file containing client socket class to manage socket
*/

#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include "xmlWriter.h"


//forward declaration of the class so it can be used
class Management;
//forward declaration of the class so it can be used
class Build;

/**
 * @class SocketClient
 * @brief The SocketClient class will be a class containing socket related items that will communicate with the AppMan Server
 */
class SocketClient : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief SocketClient The constructor that will create the object
     * @param parent The parent of the object
     */
    explicit SocketClient(Management *man, QObject *parent = 0);
    /**
      * \fn ~SocketClient();
      * The destructor
      */
    ~SocketClient();

    /**
     * @brief connectToServer The function that will connect to the AppMan Server
     * @param ipAddress The ip address on which the server is hosted
     * @param port The port on which the server listens
     */
    void connectToServer(QString ipAddress, int port);

    /**
     * @brief disconnectFromServer a function that will disconnect from the AppMan Server
     */
    void disconnectFromServer();

    /**
     * @brief writeSome A function used for debuggin purposes
     */
    void writeSome(){
        socket->write("asdasds");
    }


signals:

public slots:
    /**
     * @brief connected A function called when the socket connects to a server
     */
    void connected();

    /**
     * @brief disconnected a function called when the socket disconnects from a server
     */
    void disconnected();

    //want to know when there is bytest availaible and when tor read
    /**
     * @brief bytesWritten Function called to see how much bytes are written to the server
     * @param bytes The amount of bytes that have been written
     */
    void bytesWritten(qint64 bytes);

    //when there is info to read
    /**
     * @brief readyRead A function called each time there are information to be read from the buffer
     */
    void readyRead();


    /**
     * @brief A function called in recheck builds when master invoked it, part of phase 1
     */
    void recheckBuildsPhase1();

    /**
     * @brief A function called in recheck builds when master invoked it, part of phase 2
     */
    void recheckBuildsPhase2();

private:
    /**
     * @brief A function that will be invoked when 'CopyBuildOver' has been found in ReadyCheck
     * @param data The data that will be parsed
     */
    void copyBuildOver(QString data);


    /**
     * @brief A function that will be invoked when 'MD5Inspection' has been found in ReadyCheck
     * @param data The data that will be parsed
     */
    void sizeCheckBuild(QString data);

signals:
    void connectionEstablished();

private:
    /**
     * @brief socket The socket object that will connec to the server
     */
    QTcpSocket *socket;

    /**
     * @brief A management link that will allow backwards communication
     */
    Management *management;

    /**
     * @brief A management link that will allow backwards communication
     */
    int buildIterator;
    /**
     * @brief Will be used to loop through the builds
     */
    Build * allBuilds;
    
};

#endif // SOCKETCLIENT_H
