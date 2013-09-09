#ifndef COPYSENDERCLIENT_H
#define COPYSENDERCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QVariant>
#include <QVariantMap>
#include <QSettings>


#include "buildmd5.h"
#include "json.h"

/**
  * @class CopySenderClient
  * @brief This is a class that will be used to communcate information regarding the builds that will be copied over such as the build file MD5 values
  */
class CopySenderClient : public QObject
{
    Q_OBJECT
public:
    explicit CopySenderClient(QHostAddress hAdr, int portNumber, QObject *parent = 0);

    ~CopySenderClient();

    /**
     * \fn bool connectToHost();
     * @brief This function connects to the CopySenderServer in order to send communicate the physical build information
     * @return Returns true if the connection was successful and false if it was not
     */
    bool connectToHost();

private slots:
    /**
      * \fn void disconnectedFunction();
      * @brief the function that will be called once the socket has been disconnected
      */
    void disconnectedFunction();


    /**
      * \fn void readyReadFunction();
      * @brief The function called if there are data ready to be read
      */
    void readyReadFunction();

private:
    QString startJSONMessage();

    void appendJSONValue(QString &currentString, QString newKey, QString newValue, bool addComma);

    void endJSONMessage(QString &currentString);

    QString getMachineID();


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
     * \fn void BuildDifferent(QString data);
     * @brief A function which executes every time Build Different has been read in the request of requestHandler
     * @param data The data on which further parsing will be done
     */
    void BuildDifferent(QVariantMap jsonObject);

    /**
     * \fn void EndAllDifferences();
     * @brief The function executing when all the different build files has been received from the server
     */
    void EndAllDifferences();

    /**
     * \fn BuildMD5 *getBuildMD5Class(Build build);
     * @brief This function will calculate the MD5 of each of the files inside a build
     * @param directory The directory in which the build is contained
     * @return Returns a BuildMD5 Class containing MD5 values for each and every file
     */
    BuildMD5 *getBuildMD5Class(QString directory);


    void SendBuildMD5Class(BuildMD5 *md5Class, int i);

    void DeleteFilesList(const QVariantMap jsonObject);


private:
    QString allBuildsDirectory;

    bool firstTalk;

    int port;

    QTcpSocket *socket;

    QStringList differentBuilds;

    QHostAddress hostAddress;

};

#endif // COPYSENDERCLIENT_H
