#include "machinetest.h"

void MachineTest::initTestCase()
{
    ////?????????///////
    machine = new Slave();
}

void MachineTest::setMachineTest()
{
    machine->setMachine(150, "192.168.1.110");
    QString g = machine->getMachineIP();
    QCOMPARE(machine->getMachineID(), 150);
    QCOMPARE(machine->getMachineIP(), "192.168.1.110");
}

void MachineTest::aTest()
{
    QCOMPARE("another", "another");
}

void MachineTest::cleanupTestCase()
{
    delete machine;
}
