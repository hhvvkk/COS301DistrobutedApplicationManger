#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QObject>
#include <QTcpSocket>

class Management;


/**
  * @class Protocol
  * @brief The Protocol class is an abstract class which provides an interface to the different protocols
  */
class Protocol : public QObject
{
    Q_OBJECT
public:
    explicit Protocol(QObject *parent = 0);

    /**
     * \fn void handle(QString data, Management *man, QTcpSocket *slaveSocket) = 0;
     * @brief A virtual function to provide an interface for different protocols
     * @param data The readyRead functions output that will be parsed further in the derived classes
     * @param man The management that will be used to invoke functions on logic
     * @param masterSocket The socket that the handle function can write to
     */
    virtual void handle(QString data, Management *management, QTcpSocket *masterSocket) = 0;


};

#endif // PROTOCOL_H
