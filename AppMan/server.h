/**
* @file server.h
* @brief This is the file containing the Server run on the Application
* @author 101Solutions
* @version 2.0
*/
#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QDebug>
#include "serverthread.h"
#include "management.h"

//forward declaration so that it can be used
class Management;

/**
 * @class Server
 * @brief The Server class will be the server on which client application will connect to
 */
class Server : public QTcpServer
{
    Q_OBJECT
public:
    /**
     * \fn
     * @brief Server The constructor for the server class
     * @param m The management object sent by reference to allow two way communication
     * @param parent The parent of the Server Object
     */
    explicit Server(Management *m, QObject *parent = 0);

    /**
     * @brief startServer A function that will start the server
     */
    void startServer();

    /**
     * @brief stopServer A function that will stop the server
     */
    void stopServer();

    /**
     * @brief setPort a Function that will set the port on which the server will listen
     * @param newPort The new port on which server will listen
     */
    void setPort(int newPort);

protected:
    /**
     * @brief incomingConnection The function that is called each time a new user connects
     * @param socketID A unique value that is linked to a socket through QT
     */
    void incomingConnection(int socketID);
private:
    /**
     * @brief management The Management class to allow for two way communication
     */
    Management *management;

    /**
     * @brief port The port on which server will listen
     */
    int port;

};

#endif // SERVER_H
