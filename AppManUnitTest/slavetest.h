#ifndef SLAVETEST_H
#define SLAVETEST_H

#include "AutoTest.h"
#include "AppMan/Slave.h"

class SlaveTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    //TESTS
    void addBuildTest();
    void deleteBuildTest();

    void cleanupTestCase();

private:
    Slave *slave;
};

DECLARE_TEST(SlaveTest)

#endif // SLAVETEST_H
