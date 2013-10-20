#include "machinetest.h"
#include <QDebug>

void MachineTest::initTestCase(){

}

void MachineTest::setMachineTest(){
//    Machine *machine = 0;
//    QString supposedIP = "192.168.110.110";
//    if(machine == 0){
//        machine = new Slave(1, "192.168.1.2");
//    }
//    machine->setMachine(150, "192.168.110.110");
//    int theID = machine->getMachineID();
//    QCOMPARE(theID, 150);
//    QString getMachineIP = machine->getMachineIP();
//    QCOMPARE(getMachineIP, supposedIP);
//    delete machine;

}

void MachineTest::setMachineIPTest(){
    //Machine *Amachine = new Slave(1, "190.2.21.1");
//    QString validNewIP = "192.168.2.110";
//    Amachine->setMachineIP(validNewIP);
//    QCOMPARE(Amachine->getMachineIP(), validNewIP);
    //delete Amachine;
}

void MachineTest::setMachineIDTest(){
    //Machine *Amachine = new Slave(1, "190.2.21.1");
//    int validID = 4;
//    Amachine->setMachineID(validID);
//    QCOMPARE(Amachine->getMachineID(), validID);
    //delete Amachine;
}

void MachineTest::constructorTest(){
    Machine *newMachine = new Slave(2, "192.168.1.2");
    int id = newMachine->getMachineID();
    QString machineIP = newMachine->getMachineIP();
    QString supposedIP = "192.168.1.2";

    QCOMPARE(id, 2);
    QCOMPARE(machineIP, supposedIP);
    delete newMachine;
}

void MachineTest::cleanupTestCase(){
}
