#include "client.h"
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>

QByteArray ba("");

Client::Client(QObject *parent) :
QObject(parent)
{
socket = new QTcpSocket(this);
connect(socket, SIGNAL(connected()),
this, SLOT(on_connected()));
connect(socket, SIGNAL(readyRead()),
this, SLOT(on_readyRead()));
}

void Client::on_connected()
{    
printf("Connection established.\n");
socket->write("send\n");
}

void Client::connectToServer()
{
socket->connectToHost(QHostAddress::LocalHost, 1234);
}

void Client::on_readyRead(){
    QByteArray b = socket->readAll();
    if (strcmp(b.constData(),"done\n") == 0){
        QFile file("hey.txt");
        file.open(QFile::WriteOnly);
        file.write(ba.constData());
        file.close();
        //printf("Message: %s", ba.constData());
        socket->write("OK\n");
    }
    else {
        ba.append(b);
        socket->write("send\n");
    }
}


