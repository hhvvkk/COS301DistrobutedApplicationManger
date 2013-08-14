#include "server.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>

QFile file("hey.txt");
Server::Server(QObject *parent) :
QObject(parent)
{
server = new QTcpServer(this);
connect(server, SIGNAL(newConnection()),
this, SLOT(on_newConnection()));
file.open(QFile::ReadOnly);
size = file.size();
printf("File Size: %i", size);
count = 0;
}

void Server::listen()
{
server->listen(QHostAddress::LocalHost, 1234);
}

void Server::on_newConnection()
{
socket = server->nextPendingConnection();
if(socket->state() == QTcpSocket::ConnectedState)
{
printf("New connection established.\n");
}
connect(socket, SIGNAL(disconnected()),
this, SLOT(on_disconnected()));
connect(socket, SIGNAL(readyRead()),
this, SLOT(on_readyRead()));
printf("Attempting file transfer...\n");
}

void Server::on_disconnected()
{
printf("Connection disconnected.\n");
disconnect(socket, SIGNAL(disconnected()));
disconnect(socket, SIGNAL(readyRead()));
socket->deleteLater();
}

void Server::on_readyRead(){
QByteArray ba = socket->readAll();
printf("Message: %s", ba.constData());
if (strcmp(ba.constData(),"send\n") == 0){
    if (count < size){
        socket->write(file.read(100));
        count+=100;
        printf("Progress: %i", size-count);
    }
    else{
        socket->write("done\n");
        file.close();
    }
}
else if (strcmp(ba.constData(),"OK\n") == 0){
    socket->close();
}

}

