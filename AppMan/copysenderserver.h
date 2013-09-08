#ifndef COPYSENDERSERVER_H
#define COPYSENDERSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QStringList>
#include <QHostAddress>
#include <QNetworkInterface>
#include "buildmd5.h"
#include "json.h"
#include "copycompare.h"
#include "management.h"

/**
  * @class CopySenderServer
  * @brief This server class will be used to communicate the build information to determine which build files will be copied and which will not
  */
class CopySenderServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CopySenderServer(QStringList &diffBuilds, QStringList &diffBuildNos, Management *man, int mashId, QObject *parent = 0);

    ~CopySenderServer();

    /**
     * \fn void startServer();
     * @brief startServer A function that will start the server and return the port on which it started
     * @return Returns the port on which the server ha started
     */
    int startServer();

    /**
     * \fn void stopServer();
     * @brief stopServer A function that will stop the server
     */
    void stopServer();

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


private:
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

    void getDifferences(const QVariantMap jsonObject);

    CopyCompare *createCopyCompare(QList<QString> &keys, QVariantMap allMD5s, BuildMD5 *md5Class, QString theBuildDirectory) const;

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
    QStringList differentBuildDirectories;
    QStringList differentBuildNos;
    QTcpSocket *socket;

    /**
      * A function to ensure that the connection is valid
      */
    bool firstTalk;

    Management *management;

    int machineId;
};

#endif // COPYSENDERSERVER_H
