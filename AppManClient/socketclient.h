/**
* @file socketclient.h
* @brief This is the file containing client socket class to manage socket
* @author 101Solutions
* @version 2.0
*/

#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
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
    explicit SocketClient(QObject *parent = 0);
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

private:
    /**
     * @brief socket The socket object that will connec to the server
     */
    QTcpSocket *socket;
    
};

#endif // SOCKETCLIENT_H
