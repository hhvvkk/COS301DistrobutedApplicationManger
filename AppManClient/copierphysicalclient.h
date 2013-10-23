#ifndef COPIERPHYSICALCLIENT_H
#define COPIERPHYSICALCLIENT_H

#include <QObject>
#include <QHostAddress>
#include <QTcpSocket>
#include <QFile>
#include <QDir>

#include "compression.h"

/**
  * @class CopierPhysicalClient
  * @brief This class will connect to the physical server which will copy the file over
  */

class CopierPhysicalClient : public QObject
{
    Q_OBJECT
signals:
    /**
     * \fn void doneWritingToFile(int BuildID);
     * @brief This signal will be emitted once the files has been copied over and will notify via a boolean whether it is successful
     * @param BuildID The build number of this physical copier
     * @param success A boolean indicating whether the 7zip file is in tact
     */
    void doneWritingToFile(int BuildID, bool success);

public:
    /**
     * \fn CopierPhysicalClient(QHostAddress hAdr, int portNumber, int bNumber, int theMagicNumber, QObject *parent = 0);
     * @brief The constructor for the physicalClient
     * @param hAdr The host address of the machine to which it will connect
     * @param portNumber The port number where the server will connect to
     * @param theMagicNumber The magic number which will determine how the build is extracted and how deep the file will be traversed to keep structure of files
     */
    explicit CopierPhysicalClient(QHostAddress hAdr, int portNumber, int bNumber, int theMagicNumber, QObject *parent = 0);

    ~CopierPhysicalClient();


    /**
     * \fn bool connectToHost();
     * @brief This function connects to the CopySenderServer in order to send communicate the physical build information
     * @return Returns true if the connection was successful and false if it was not
     */
    bool connectToHost();


    /**
     * \fn void PhysicalServerDone(const QVariantMap jsonObject);
     * @brief A function which will be called indicating that the server is done sending the file
     */
    void PhysicalServerDone();


    /**
     * \fn int getBuildID();
     * @brief A function returning the build number for which this physical copy is taking place
     * @return Return the build no to which the copy takes place
     */
    int getBuildID();

    /**
      * \fn void writeToFile();
      * @brief A function which writes the buffer that has been read to the file
      */
    void writeToFile();

    /**
      * \fn void removeZipFile();
      * @brief This function will remove the zip file to which the build is being copied over if it exist
      */
    void removeZipFile();

private:
    /**
     * \fn bool zipInTact(QString zipPath);
     * @brief A function which checks if a zip is in tact
     * @return Returns true if the zip file is in tact, else returns false
     */
    bool zipInTact();


    /**
     * \fn void extractZipToDirectory();
     * @brief Extract the files to the extract directory
     */
    void extractZipToDirectory();

private slots:
    /**
      * \fn void disconnectedFunction();
      * @brief the function that will be called once the socket has been disconnected
      */
    void disconnectedFunction();

    /**
      * \fn void readyReadFunction();
      * @brief The function called if there is data to be read(which is the file to be copied over the network)
      */
    void readyReadFunction();

private:
    int port;

    QHostAddress hostAddress;

    QTcpSocket *socket;

    QString allBuildsDirectory;
    QString compressDirectory;
    QString extractDirectory;

    int BuildID;

    int amountRead;

    QByteArray fileBuffer;

    int magicNumber;
};

#endif // COPIERPHYSICALCLIENT_H
