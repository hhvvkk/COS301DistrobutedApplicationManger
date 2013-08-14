#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class QTcpSocket;

class Client : public QObject
{
Q_OBJECT
public:
explicit Client(QObject *parent = 0);
void connectToServer();
signals:

public slots:
void on_connected();
void on_readyRead();
private:
QTcpSocket* socket;
};

#endif // CLIENT_H
