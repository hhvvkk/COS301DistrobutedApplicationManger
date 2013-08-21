/**
* @file network.h
* @brief This is the file containing the network to help manage connections
*/

#ifndef NETWORK_H
#define NETWORK_H

#include <QString>
#include "socketclient.h"

//forward declaration of Management for use
class Management;


/**
 * @class Network
 * @brief The Network class will be used to manage some of the network related aspects of the AppMan Client application
 */
class Network
{
public:
    /**
     * @brief Network The constructor for the class
     */
    Network(Management *man);

    /**
     * @brief connecToServer Function that will call the connect on the SocketClass (SocketClient)
     */
    void connecToServer();

    /**
     * @brief setConnectionDetails A function that will set the connection details as specified by parameters
     * @param ip The ip address to which one will connect
     * @param port The port on which the server will be listening
     */
    void setConnectionDetails(QString ip, int port);

    /**
     * @brief disconnect Function that will disconnect from the AppMan Server
     */
    void disconnect();

    /**
     * @brief writeSOme function writing some text to server for debugging
     */
    void writeSOme(){
        socketClient->writeSome();
    }

private:
    /**
     * @brief socketClient The socket object containing socket related items and management
     */
    SocketClient *socketClient;
    /**
     * @brief serverPort The port on which the AppMan server will be listening
     */
    int serverPort;
    /**
     * @brief IpAddress The IP Address on which the AppMan server will be run
     */
    QString IpAddress;
};

#endif // NETWORK_H
