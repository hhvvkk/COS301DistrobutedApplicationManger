/**
* @file server.h
* @brief This is the file containing the Server run on the Application
*/
#ifndef SERVER_H
#define SERVER_H

#include <QDebug>
#include <QTcpServer>
#include <QSettings>
#include <QMessageBox>
#include <QIcon>

#include "management.h"
#include "serverthread.h"

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
     * \fn Server(Management *m, QObject *parent = 0);
     * @brief Server The constructor for the server class
     * @param m The management object sent by reference to allow two way communication
     * @param parent The parent of the Server Object
     */
    explicit Server(Management *m, QObject *parent = 0);

    /**
     * \fn void startServer();
     * @brief startServer A function that will start the server
     */
    void startServer();

    /**
     * \fn void stopServer();
     * @brief stopServer A function that will stop the server
     */
    void stopServer();

    /**
     * \fn void setPort(int newPort);
     * @brief setPort a Function that will set the port on which the server will listen
     * @param newPort The new port on which server will listen
     */
    void setPort(int newPort);

protected:
    /**
     * \fn void incomingConnection(int socketID);
     * @brief incomingConnection The function that is called each time a new user connects
     * @param socketID A unique value that is linked to a socket through QT
     */
    void incomingConnection(int socketID);

    void showMessage(QString message, QString flag = "");

private:
    /**
     * @var management
     * @brief management The Management class to allow for two way communication
     */
    Management *management;

    /**
     * @var management
     * @brief port The port on which server will listen
     */
    int port;

};

#endif // SERVER_H
