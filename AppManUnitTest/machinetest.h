#ifndef MACHINETEST_H
#define MACHINETEST_H

#include "AutoTest.h"
#include "AppMan/Slave.h"
#include "AppMan/Machine.h"

class MachineTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();


    //TESTS
    void setMachineTest();
    void constructorTest();
    void setMachineIPTest();
    void setMachineIDTest();


    void cleanupTestCase();
};

DECLARE_TEST(MachineTest)

#endif // MACHINETEST_H
