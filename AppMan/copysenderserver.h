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
      */
    void copySenderServerDone(CopySenderServer* thsServer);

public:
    explicit CopySenderServer(QStringList *diffBuilds, QStringList *diffBuildNos, Management *man, int mashId, QObject *parent = 0);

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
     * \fn void PhysicalServerDoneNotify(int buildNo);
     * @brief Function which is invoked if the physical copier is done copying the file and emits a signal
     * @param buildNo The number of the build being copied over
     */
    void PhysicalServerDoneNotify(int buildNo);


    /**
     * \fn void PhysicalServerDoneNotify(int buildNo);
     * @brief Function which is invoked if the physical copier is done copying the file and emits a signal
     * @param index The index where the copy currently is
     * @param bufferSize The total buffer size which will be sent across the network
     * @param buildNo The number of the build being copied over
     */
     void notifyProgress(int index, int bufferSize, int buildNo);


     /**
      * \fn void nextInQueue(int port);
      * @brief A function connected to the nextInQueue of the copyQueue which will emit the port that the client should connect to
      * @param port The port on which the next in queue is waiting on
      * @param buildNo The build number for which the next in queue is meant for
      */
     void nextInQueue(int port, int buildNo);

     /**
      * \fn void queueFinished();
      * @brief The function connected to a queue signal which indicates that it is empty
      * @param theCopyQueue The queue which was finished copying
      */
     void queueFinished(CopyQueue * theCopyQueue);

private:
    QString startJSONMessage();

    void appendJSONValue(QString &currentString, QString newKey, QString newValue, bool addComma);

    void endJSONMessage(QString &currentString);


    /**
     * \fn void handle(QString data);
     * @brief A function which strips the slashes from data and splits various requests that may be together into separate requests and calls requestHandler(theData)
     * @param data The data that will be used and parsed
     */
    void handle(QString data);

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

    void BuildFileSumMD5(const QVariantMap jsonObject);

    CopyCompare *createCopyCompare(QList<QString> &keys, QVariantMap allMD5s, BuildMD5 *md5Class, QString theBuildDirectory) const;

    /**
     * \fn void SendDifferences();
     * @brief A function which creates the physical copysenderserver by which it copies the files over...
     * @param buildNo The build number for which the physical copying will be created
     */
    void createPhysicalCopier(int buildNo);

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
    QStringList *differentBuildNos;
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
