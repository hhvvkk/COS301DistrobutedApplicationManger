#include "managementtest.h"

void ManagementTest::initTestCase()
{
}

void ManagementTest::aTest()
{
    QVERIFY(1 == 2);
}

void ManagementTest::test2()
{
    QVERIFY(1 == 0);
    QVERIFY(1 == 4);
    QVERIFY(1 == 5);
    QVERIFY(1 == 3);
}

void ManagementTest::cleanupTestCase()
{
}
