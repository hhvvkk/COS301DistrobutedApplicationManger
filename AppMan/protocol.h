#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

class Management;

class Protocol : public QObject
{
    Q_OBJECT
public:
    /**
     * \fn Protocol(QObject *parent = 0);
     * @brief The constructor for the Protocol
     */
    explicit Protocol(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket) = 0;
     * @brief A virtual function to provide an interface for different protocols
     * @param jsonObject A QVariantMap which contains the values from the json string
     * @param man The management that will be used to invoke functions on logic
     * @param slaveSocket The socket that the handle function can write to
     */
    virtual void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket) = 0;

protected:
    /**
     * \fn QString startJSONMessage();
     * @brief A function returning a bracket for the beginning of a json message
     * @return Returns a starting bracket for communication
     */
    QString startJSONMessage();

    /**
     * \fn void appendJSONValue(QString &currentString, QString newKey, QString newValue, bool addComma);
     * @brief A function which will add a new value with its key to the current jsonstring
     * @param currentString The current string that a value needs to be added to
     * @param newKey The key of the value that will be added
     * @param newValue The value that will be added in the form of QString
     * @param addComma A boolean indicating if a comma needs to be added to the end of the appended json value
     */
    void appendJSONValue(QString &currentString, QString newKey, QString newValue, bool addComma);

    /**
     * \fn void endJSONMessage(QString &currentString);
     * @brief A function which appends the bracket to end the message and appends the slashes for communication
     */
    void endJSONMessage(QString &currentString);
};

#endif // PROTOCOL_H
