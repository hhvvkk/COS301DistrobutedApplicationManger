#ifndef SLAVETEST_H
#define SLAVETEST_H

#include "AutoTest.h"

class SlaveTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test1();
    void anotherTest();
    void cleanupTestCase();
};

DECLARE_TEST(SlaveTest)

#endif // SLAVETEST_H
