#ifndef MANAGEMENT_H
#define MANAGEMENT_H

#include <QString>
#include "network.h"

class Management : public QObject
{
    Q_OBJECT
public:
    Management();
    ~Management();
    void connectToServer(QString ipAddress, int port);
    void disconnectFromServer();
    void writeSomedata(){
        network->writeSOme();
    }

private:
    Network *network;
};

#endif // MANAGEMENT_H
