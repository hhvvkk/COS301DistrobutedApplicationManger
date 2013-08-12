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

#include "Machine.h"
#include "management.h"
#include "Slave.h"

//forward declaration of Management so it can be used
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
     * @param m the management object to harbour two way communications
     * @param parent The parent of the QThread object
     */
    explicit ServerThread(int ID,Management *m, QObject *parent = 0);

    /**
     * @brief run Function that will run once thread is started
     */
    void run();

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
     * @brief management An Object to harbour two way communications
     * @brief machine The machine for which this thread runs
     * @brief firstTalk Whether or not it is the first time the client talks
     */
    QTcpSocket *socket;
    int socketID;
    Management *management;
    Machine *machine;
    bool firstTalk;
};

#endif // SERVERTHREAD_H
