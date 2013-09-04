#ifndef COPYSENDERSERVER_H
#define COPYSENDERSERVER_H

#include <QTcpServer>
#include <QStringList>

class CopySenderServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit CopySenderServer(QStringList &diffBuilds, QObject *parent = 0);

    /**
     * \fn void startServer();
     * @brief startServer A function that will start the server
     */
    void startServer();

    /**
     * \fn void stopServer();
     * @brief stopServer A function that will stop the server
     */
    void stopServer();

signals:

public slots:

private:
    int port;
    QStringList differentBuildDirectories;

};

#endif // COPYSENDERSERVER_H
