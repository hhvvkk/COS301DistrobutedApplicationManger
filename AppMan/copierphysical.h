#ifndef COPIERPHYSICAL_H
#define COPIERPHYSICAL_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QTimer>
#include <QFuture>
#include <QtConcurrentRun>

/**
 * @class CopierPhysical
 * @brief This is a class that will contain a server and a physical link to copy over the file from this machine to the other machine
 */
class CopierPhysical : public QTcpServer
{
    Q_OBJECT
signals:
    /**
      * \fn void copierPhysicalDone();
      * @brief A signal that will be emitted once the file has completely been sent across the network
      * @param buildID The build ID of the build which is done sending
      */
    void copierPhysicalDone(int buildID);


    /**
      * \fn void copierPhysicalDone();
      * @brief A signal that will be emitted when the allotted time of the progress timer has been ended
      * @param currentIndex The current i (index in the buffer)
      * @param bufferSize The full size of the buffer being sent to the client
      * @param BuildID The number of the build to which the signal reffers to
      */
    void notifyProgress(int currentIndex, int bufferSize, int BuildID);

public:

    /**
      * \fn CopierPhysical(int machID, int buildID, QString pathForZip, QObject *parent = 0);
      * @brief Constructor for the physical copier
      * @param machID The current ID of the machine
      * @param pathForZip Where the zip is located to be copied
      * @param buildID The number of the build which will be copied by this copier
      */
    explicit CopierPhysical(int machID, int buildID, QString pathForZip, QObject *parent = 0);


    ~CopierPhysical();


    /**
      * @fn int startServer();
      * @brief Start the server and copies the file over to the first connecting computer
      */
    int startServer();

    /**
     * \fn void stopServer();
     * @brief stopServer A function that will stop the server
     */
    void stopServer();

    int getBuildID();

protected:
    /**
     * \fn void incomingConnection(int socketID);
     * @brief incomingConnection The function that is called each time a new client connects
     * @param socketID A unique value that is linked to a socket through QT
     */
    void incomingConnection(int socketID);


private slots:
    /**
     * \fn void destroyServer();
     * @brief stopServer A function cleaning up the server and executing once socket is disconnected
     */
    void destroyServer();


    /**
     * \fn void signalNotifyProgress();
     * @brief A function invoked by notifyTimer which will signal the notifyProgress function
     */
    void signalNotifyProgress();

    void readyReadFunction();

private:

    /**
     * \fn void writeFileOverNetwork();
     * @brief A function which physically writes the file over the network(as captured in the buffer)
     */
    void writeFileOverNetwork();


    /**
     * \fn void destroyServer();
     * @brief A function which starts the process of copying the file over the network
     */
    void initiateCopyOver();

    /**
     * \fn QByteArray concurrentFileRead(QFile *fileToRead);
     * @brief A function to read from the file into a byte array
     * @return Returns the QByteArray of the file
     * @param fileToRead The file which will be read
     */
    QByteArray concurrentFileRead(QFile *fileToRead);

private:
    /**
      * The machineID which is being copied to
      */
    int machineID;

    /**
      * The build ID being copied over
      *
      */
    int BuildID;

    /**
      * A timer used to notify the current progress of the build being copied over
      */
    QTimer notifyTimer;


    /**
      * A socket which will be used to copy the file over the network
      */
    QTcpSocket *socket;

    /**
      * A root path to where the zip file will be located
      *
      */
    QString zipPath;


    /**
      * A File object to the zip file to be copied over the network
      */
    QFile *theZipFile;


    /**
      * A buffer to read all the bytes from the file from
      */
    QByteArray buffer;

    /**
      * The maximum amount of bytes that may be sent across the network
      */
    int maxPerSize;

    /**
      * A placeholder to continue with sending of the file
      *
      */
    int i;

};

#endif // COPIERPHYSICAL_H
