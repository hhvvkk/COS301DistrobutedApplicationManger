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
     * @brief ServerThread The constructor for the Server thread
     * @param ID The unique socket id that will be linked to a new connecting client on Server inside incommingConnection(socketiD)
     * @param parent The parent of the QThread object
     */
    explicit ServerThread(int ID, Management *man, QObject *parent = 0);

    /**
     * @brief run Function that will run once thread is started
     */
    void run();

    QTcpSocket *getSocket();

public slots:
    /**
     * @brief readyReadFunction A function that will be run each time the thread obtains new data from client
     */
    void readyReadFunction();

    /**
     * @brief disconnectedFunction The function that will be called once the client disconnects
     */
    void disconnectedFunction();

private:
    /**
     * @brief socket
     * @brief socketID
     */
    QTcpSocket *socket;
    int socketID;
   // bool firstTalk;
    //Build* recheckBuilds;

    ProtocolHandler *protocolHandler;
};

#endif // SERVERTHREAD_H
