#ifndef PROTOAPPLIST_H
#define PROTOAPPLIST_H

#include "protocol.h"
#include <QMapIterator>

class ProtoAppList : public Protocol
{
    Q_OBJECT
public:
    explicit ProtoAppList(QObject *parent = 0);
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
    void requestAppList(QTcpSocket *slaveSocket, Management *man);
    void appListFollows(QVariantMap jsonObject, QTcpSocket *slaveSocket);
private:
    int machineID;
};

#endif // PROTOAPPLIST_H
