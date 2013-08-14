#include <QtCore/QCoreApplication>
#include "client.h"

int main(int argc, char *argv[])
{
QCoreApplication a(argc, argv);
Client c;
c.connectToServer();
return a.exec();
}
