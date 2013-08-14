#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QFile>
class QTcpServer;
class QTcpSocket;

class Server : public QObject
{
Q_OBJECT
public:
explicit Server(QObject *parent = 0);
void listen();
signals:

public slots:
void on_newConnection();
void on_disconnected();
void on_readyRead();
private:
QTcpServer* server;
QTcpSocket* socket;
int size;
int count;

};

#endif // SERVER_H
