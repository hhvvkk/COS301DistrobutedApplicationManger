#ifndef PROTOCOLTEST_H
#define PROTOCOLTEST_H

#include <QObject>
#include "AutoTest.h"
#include <AppMan/protocol.h>
#include <AppMan/protoconnect.h>

class ProtocolTest : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();


//    void startJSONMessageTest();
//    void appendJSONValueTest();
//    void endJSONMessageTest();
//    void sendJSONMessageTest();

    void cleanupTestCase();
private:
    Protocol *connectProtocol;

};

DECLARE_TEST(ProtocolTest)

#endif // PROTOCOLTEST_H
