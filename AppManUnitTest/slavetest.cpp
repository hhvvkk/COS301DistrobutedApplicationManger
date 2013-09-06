#include "slavetest.h"

void SlaveTest::initTestCase()
{
}

void SlaveTest::test1()
{
    QCOMPARE(42, 40 + 2);
}

void SlaveTest::anotherTest()
{
    QCOMPARE("Hello", "World!");
}

void SlaveTest::cleanupTestCase()
{
}
