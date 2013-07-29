#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QString>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = 0);
    ~SocketClient();
    void connectToServer(QString ipAddress, int port);
    void disconnectFromServer();
    void writeSome(){
        socket->write("asdasds");
    }

signals:

public slots:
    void connected();
    void disconnected();

    //want to know when there is bytest availaible and when tor read
    void bytesWritten(qint64 bytes);
    //when there is info to read
    void readyRead();

private:
    QTcpSocket *socket;
    
};

#endif // SOCKETCLIENT_H
