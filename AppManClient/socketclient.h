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

#include "protocolhandler.h"

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
     * @brief disconnected a function called when the socket disconnects from a server
     */
    void disconnected();

    //when there is info to read
    /**
     * @brief readyRead A function called each time there are information to be read from the buffer
     */
    void readyRead();


private:
    void recheckBuildsPhase1();


private:
    /**
     * @brief socket The socket object that will connec to the server
     */
    QTcpSocket *socket;

    /**
     * @brief A management link that will allow backwards communication
     */
    Management *management;



    ProtocolHandler *protocolHandler;
    
};

#endif // SOCKETCLIENT_H
