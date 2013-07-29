#include "management.h"

Management::Management()
{
    network = new Network();
}


Management::~Management()
{
    delete network;
}

void Management::connectToServer(QString ipAddress, int port){
    network->setConnectionDetails(ipAddress, port);
    network->connecToServer();
}

void Management::disconnectFromServer(){
    network->disconnect();
}
