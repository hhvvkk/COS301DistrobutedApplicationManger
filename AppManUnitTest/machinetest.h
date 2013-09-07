#ifndef MACHINETEST_H
#define MACHINETEST_H

#include "AutoTest.h"
#include "../AppMan/Slave.h"

class MachineTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void setMachineTest();
    void aTest();
    void cleanupTestCase();

private:
    Machine *machine;
};

DECLARE_TEST(MachineTest)

#endif // MACHINETEST_H
