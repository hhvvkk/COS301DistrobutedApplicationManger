#ifndef COPYSENDERSERVER_H
#define COPYSENDERSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QVariant>
#include <QVariantMap>
#include <QMutex>
#include <QFuture>

#include "buildmd5.h"
#include "json.h"
#include "copycompare.h"
#include "management.h"
#include "compression.h"
#include "copierphysical.h"
#include "copyqueue.h"


/**
  * @class CopySenderServer
  * @brief This server class will be used to communicate the build information to determine which build files will be copied and which will not
  */
class CopySenderServer : public QTcpServer
{
    Q_OBJECT
signals:
    /**
      * \fn void copySenderServerDone(CopySenderServer* thsServer);
      * @brief This signal is emitted once the copySenderServer is done sending the build files
      * @param thsServer The server which is done
      */
    void copySenderServerDone(CopySenderServer* thsServer);

    /**
      * \fn void fullySynchronised(int intBuildID, int machineId, Management *management);
      * @brief Signal emitted when a certain build is fully synchronised on the slave machine
      * @param intBuildID the ID of the build which is fully synchronised
      * @param machineId The ID of the machien which has a full synchronisation
      * @param management The management class which will be notified of the syncrhonisation
      */
    void fullySynchronised(int intBuildID, int machineId, Management *management);
public:
    explicit CopySenderServer(QStringList *diffBuilds, QStringList *diffBuildIDs, Management *man, int mashId, QObject *parent = 0);

    /**
     * @fn void loadCompressPath();
     * @brief A function which will load the compress location on the disk where the files will be compressed before sending files accross network
     */
    void loadCompressPath();

    ~CopySenderServer();

    /**
     * \fn void startServer();
     * @brief A function that will start the server and return the port on which it started
     * @return Returns the port on which the server ha started
     */
    int startServer();

    /**
     * \fn void stopServer();
     * @brief stopServer A function that will stop the server
     */
    void stopServer();


    /**
     * \fn bool isBusyDeleting();
     * @brief A function that can be queried to find out if there are still files being deleted such as the zip file
     * @return Returns true if there are files still being deleted
     */
    bool isBusyDeleting();
private slots:
    /**
     * \fn void destroyServer();
     * @brief stopServer A function cleaning up the server and executing once socket is disconnected
     */
    void destroyServer();

    /**
     * \fn void readyReadFunction();
     * @brief The ready read function to get information from the CopySenderClient from appman class
     */
    void readyReadFunction();


    /**
     * \fn void PhysicalServerDoneNotify(int BuildID);
     * @brief Function which is invoked if the physical copier is done copying the file and emits a signal
     * @param BuildID The ID of the build being copied over
     */
    void PhysicalServerDoneNotify(int BuildID);


    /**
     * \fn void PhysicalServerDoneNotify(int BuildID);
     * @brief Function which is invoked if the physical copier is done copying the file and emits a signal
     * @param index The index where the copy currently is
     * @param bufferSize The total buffer size which will be sent across the network
     * @param BuildID The ID of the build being copied over
     */
     void notifyProgress(int index, int bufferSize, int BuildID);

     /**
      * \fn void nextInQueue(int port);
      * @brief A function connected to the nextInQueue of the copyQueue which will emit the port that the client should connect to
      * @param port The port on which the next in queue is waiting on
      * @param BuildID The build ID for which the next in queue is meant for
      */
     void nextInQueue(int port, int BuildID);

     /**
      * \fn void queueFinished();
      * @brief The function connected to a queue signal which indicates that it is empty
      * @param theCopyQueue The queue which was finished copying
      */
     void queueFinished(CopyQueue * theCopyQueue);

private:
     /**
      * \fn QString startJSONMessage();
     * @brief Returns a jsonMessage for the started QString as the started jsonstring to send across network
     * @return returns a QString containing the jsonMessage that was started
     */
    QString startJSONMessage();

    /**
     * \fn QString startJSONMessage();
    * @brief Appends the newkey as a jsonValue to the json current string and will add a comma depending on the boolean
    * @param currentString The current json message string as it is progressing
    * @param newKey The key of the item to be sent
    * @param newValue The value for the corrosponding new key
    * @param addComma A boolean indicating whether or not to add a comma to the value(Hint: end of json no comma is required)
    */
    void appendJSONValue(QString &currentString, QString newKey, QString newValue, bool addComma);

    /**
     * \fn void endJSONMessage(QString &currentString);
    * @brief A function to end the jsonMessage that was started
    * @param currentString The current json message string as it is progressing
    */
    void endJSONMessage(QString &currentString);

    /**
     * \fn void sendJSONMessage(QTcpSocket *slaveSocket, QString jsonMessage);
    * @brief This writes the jsonMessage to the slaveSocket as specified
    * @param jsonMessage The JsonMessage that will be written to  the network
    * @param slaveSocket The slave socket that will be written to
    */
    void sendJSONMessage(QTcpSocket *slaveSocket, QString jsonMessage);


    /**
     * \fn void handle(QString data);
     * @brief A function which handles the incoming data accordingly
     * @param data The data that will be used and parsed
     */
    void handle(QString data);


    /**
     * \fn QStringList splitRequests(QString data);
     * @brief A function which strips the slashes from data and splits various requests that may be together into separate requests and places those request in the return object
     * @param data The data that will be used and parsed
     * @return The different requests that will be returned inside an object
     */
    QStringList splitRequests(QString data);

    /**
     * \fn void requestHandler(QString data);
     * @brief This function will be invoked by the handle function after the data string has been parsed to remove slashes. The function will invoke a protocol depending on what has been found
     * @param data The data on which further parsing will be done
     */
    void requestHandler(QString data);

    /**
     * \fn void SendDifferences();
     * @brief A function in which the server will notify the client with all the builds that are different
     */
    void SendDifferences();

    /**
     * \fn void SendDifferences();
     * @brief A function which ultimately sends creates the Physical copiers in order to send the files to the client
     */
    void BuildFileSumMD5(const QVariantMap jsonObject);

    /**
     * \fn void SendDifferences();
     * @brief Creates a CopyCompare class by comparing the build files on the slave machine to the one on the master
     * @param keys The keys of the json md5 values of the slave(i.e. the directories)
     * @param allMD5s The json values(MD5 Sum values for each of the files)
     * @param theBuildDirectory The directory where the files are contained
     * @param md5Class The class to be used for the comparison containing all the md5 values from the master machine
     */
    CopyCompare *createCopyCompare(QList<QString> &keys, QVariantMap allMD5s, BuildMD5 *md5Class, QString theBuildDirectory) const;

    /**
     * \fn void SendDifferences();
     * @brief A function which creates the physical copysenderserver by which it copies the files over...
     * @param BuildID The build ID for which the physical copying will be created
     */
    void createPhysicalCopier(int BuildID);

    /**
     * \fn void addToQueue(CopierPhysical *physicalCopier);
     * @brief A function to add the newly created physical copier to the current queue or a newly created queue
     * @param physicalCopier The physical copier which will be copied over
     */
    void addToQueue(CopierPhysical *physicalCopier);

    /**
     * \fn void NotifyCopySuccess(const QVariantMap jsonObject);
     * @brief A function which is called when the client copy sender notifies this machien if the file copy has been a success
     */
    void NotifyCopySuccess(const QVariantMap jsonObject);

    /**
     * @brief Where the actual calculation of the progress is done and management is notified apon that
     * @param index The index where the copy currently is
     * @param bufferSize The total buffer size which will be sent across the network
     * @param BuildID The ID of the build being copied over
     */
    void calculateProgress(int index, int bufferSize, int BuildID);
protected:
    /**
     * \fn void incomingConnection(int socketID);
     * @brief incomingConnection The function that is called each time a new client connects
     * @param socketID A unique value that is linked to a socket through QT
     */
    void incomingConnection(int socketID);

private:
    //this ip address is to be used for reference purposes
    //so that can let the mainform know who has what percentage
    //of builds
    QString ipAddress;
    QStringList *differentBuildDirectories;
    QStringList *differentBuildIDs;
    QTcpSocket *socket;

    /**
      * A function to ensure that the connection is valid
      */
    bool firstTalk;

    Management *management;

    int machineId;

    /**
      * A path to the location on hard drive where the files will be compressed to
      */
    QString fileCompressPath;

    CopyQueue *copierQueue;

    QMutex lock;

    /**
      * @brief a boolean indicatin if there are still files left to delete which the server is most likely busy deleting
      *
      */
    bool zipFilesToDelete;
};

#endif // COPYSENDERSERVER_H
