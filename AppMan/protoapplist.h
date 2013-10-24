#ifndef PROTOAPPLIST_H
#define PROTOAPPLIST_H

#include "protocol.h"

class ProtoAppList : public Protocol
{
    Q_OBJECT
public:
    explicit ProtoAppList(QObject *parent = 0);
    void handle(QVariantMap jsonObject, Management *man, QTcpSocket *slaveSocket);
    void requestAppList(QTcpSocket *slaveSocket);
    void appListFollows(QVariantMap jsonObject, QTcpSocket *slaveSocket, Management *man);
};

#endif // PROTOAPPLIST_H
