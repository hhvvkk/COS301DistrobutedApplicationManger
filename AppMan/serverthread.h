/**
* @file serverthread.h
* @brief This is the file containing the thread that are run for each client connecting
*/

#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>


#include "protocolhandler.h"

class Management;

/**
 * @class ServerThread
 * @brief The ServerThread class will be constructed each time a client application will connects to harbour separate communication for all clients connecting
 */
class ServerThread : public QThread
{
    Q_OBJECT
public:
    /**
     * \fn ServerThread(int ID, Management *man, QObject *parent = 0);
     * @brief ServerThread The constructor for the Server thread
     * @param ID The unique socket id that will be linked to a new connecting client on Server inside incommingConnection(socketiD)
     * @param parent The parent of the QThread object
     */
    explicit ServerThread(int ID, Management *man, QObject *parent = 0);

    /**
     * \fn run();
     * @brief run Function that start the thread for a client
     */
    void run();


    /**
     * \fn QTcpSocket *getSocket();
     * @brief Function that returns the socket
     * @return The socket of this class which will be the clients socket
     */
    QTcpSocket *getSocket();

public slots:
    /**
     * \fn void readyReadFunction();
     * @brief A function that will be run each time the thread obtains new data from client
     */
    void readyReadFunction();

    /**
     * \fn void disconnectedFunction();
     * @brief The function that will be called once the client disconnects
     */
    void disconnectedFunction();

private:

    /**
     * @var socket
     * @brief The client socket
     */
    QTcpSocket *socket;


    /**
     * @var protocolHandler
     * @brief The id of the socket that was made
     */
    int socketID;

    /**
     * @var protocolHandler
     * @brief The protocol handler that will be used to invoke protocols
     */
    ProtocolHandler *protocolHandler;
};

#endif // SERVERTHREAD_H
