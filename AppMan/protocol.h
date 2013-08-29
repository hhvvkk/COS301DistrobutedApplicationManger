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
     * @param data The readyRead functions output that will be parsed further
     * @param man The management that will be used to invoke functions on logic
     * @param slaveSocket The socket that the handle function can write to
     */
    virtual void handle(QString data, Management *man, QTcpSocket *slaveSocket) = 0;

signals:

public slots:

};

#endif // PROTOCOL_H
