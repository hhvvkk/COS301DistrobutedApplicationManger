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
    explicit SocketClient(ProtocolHandler *protoHandler, QTcpSocket * newSocket, QObject *parent = 0);
    /**
      * \fn ~SocketClient();
      * The destructor
      */
    ~SocketClient();

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
    /**
     * @var socket
     * @brief socket The socket object that will connec to the server
     */
    QTcpSocket *socket;

    /**
     * @var protocolHandler
     * @brief The protocolhandler which will use the protocols
     */
    ProtocolHandler *protocolHandler;

};

#endif // SOCKETCLIENT_H
