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

#include "Build.h"

//forward declaration of Management so it can be used
class Management;
class Machine;

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
    explicit ServerThread(int ID, Management *m, QObject *parent = 0);

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

    /**
     * \fn void copyBuildOver(int buildId, QString buildName);
     * @brief A function that will initiate a copy over from master to slave machine
     * @param buildId The new id to send to the machine
     * @param buildName The new name to send to the machine
     */
    void copyBuildOver(int buildId, QString buildName);


private:
    /**
     * \fn void copyBuildOver(int buildId, QString buildName);
     * @brief A function invoked when the ReadyRead observes 'GotABuild' in the data string and parses it
     * @param data The data to be parsed by this function
     */
    void GotABuild(QString data);


    /**
     * \fn void copyBuildOver(int buildId, QString buildName);
     * @brief A function invoked when the ReadyRead observes 'RecheckBuilds' in the data string and parses it
     * @param data The data to be parsed by this function
     */
    void RecheckBuilds(QString data);


    /**
     * \fn void copyBuildOver(int buildId, QString buildName);
     * @brief A function invoked when the ReadyRead observes 'RecheckDone' in the data string and parses it
     * @param data The data to be parsed by this function
     */
    void RecheckDone(QString data);


    /**
     * \fn void copyBuildOver(int buildId, QString buildName);
     * @brief A function invoked when the ReadyRead observes 'Rechecker' in the data string and parses it
     * @param data The data to be parsed by this function
     */
    void Rechecker(QString data);

    /**
     * \fn void SizeCheckAllBuilds();
     * @brief This function finds the md5 value for each build and if it is different, the CopyBuild function will be called
     */
    void SizeCheckAllBuilds();

    /**
     * \fn void SizeCheckBuild();
     * @brief This function will find and return the MD5 value for a specific build
     ..............>>>>DOCUKMeTNATIONSDS:S:AS:DS:ADGKDGKNGJRONO   <--LOLWUT??
     */
    QString sizeCheckBuild(QString buildNo);
private:
    /**
     * @brief socket
     * @brief socketID
     * @brief firstTalk Whether or not it is the first time the client talks
     */
    QTcpSocket *socket;
    int socketID;
    bool firstTalk;
    Management *management;
    Machine *machine;
    //Build* recheckBuilds;
};

#endif // SERVERTHREAD_H
