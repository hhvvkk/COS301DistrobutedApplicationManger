#include "protocoltest.h"


void ProtocolTest::initTestCase(){
    connectProtocol = new ProtoConnect();
}

//void ProtocolTest::startJSONMessageTest(){
//}


//void ProtocolTest::appendJSONValueTest(){

//}


//void ProtocolTest::endJSONMessageTest(){

//}


//void ProtocolTest::sendJSONMessageTest(){

//}


void ProtocolTest::cleanupTestCase(){
    connectProtocol->deleteLater();
}
