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
    explicit Protocol(QObject *parent = 0);

    virtual void handle(QString data, Management *man, QTcpSocket *slaveSocket) = 0;

signals:

public slots:

};

#endif // PROTOCOL_H
