#ifndef PROTOCOPYOVER_H
#define PROTOCOPYOVER_H

#include "protocol.h"

class ProtoCopyOver : public Protocol
{
    Q_OBJECT
public:
    explicit ProtoCopyOver(QObject *parent = 0);

    void handle(QString data, Management *man, QTcpSocket *slaveSocket);

    void copyBuildOver(int buildId, QString buildName, QTcpSocket *slaveSocket);
signals:

public slots:

private:
    /**
     * \fn void GotABuild(int buildId, QString buildName);
     * @brief A function invoked when the ReadyRead observes 'GotABuild' in the data string and parses it
     * @param data The data to be parsed by this function
     */
    void GotABuild(QString data, Management *man, QTcpSocket *slaveSocket);


};

#endif // PROTOCOPYOVER_H
