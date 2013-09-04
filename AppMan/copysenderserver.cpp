#include "copysenderserver.h"

CopySenderServer::CopySenderServer( QStringList &differentB, QObject *parent) :
    QTcpServer(parent)
{
    differentBuildDirectories = differentB;

    qDebug()<<"All the different Builds follows::in CopySenderServer";
    for(int i = 0; i < differentBuildDirectories.size(); i++){
        qDebug()<<"DifferentBuild="<<differentBuildDirectories.at(i);
    }
}


void CopySenderServer::startServer(){
    if(!this->listen(QHostAddress::Any, port)){
        qDebug() << "Could not start server";
    }
    else{
        qDebug() << "SenderServerListening..."<<port;
    }
}


void CopySenderServer::stopServer(){
    CopySenderServer::close();
}
